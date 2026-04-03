#!/usr/bin/env python3
"""Composite random house sprites from img/houses into a WxW isometric cluster."""
from __future__ import annotations

import argparse
import os
import random
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
HOUSES_DIR = ROOT / "img" / "houses"
FALLBACK = ROOT / "img" / "small_city.png"

# Composite at this scale (nearest-neighbor), then Lanczos down to 1× — smooths silhouette / stair-steps.
SUPERSAMPLE = 2
_LANCZOS = getattr(Image, "Resampling", Image).LANCZOS


def list_house_paths() -> list[Path]:
    exts = {".png", ".webp"}
    paths = [
        p
        for p in HOUSES_DIR.iterdir()
        if p.is_file() and p.suffix.lower() in exts and not p.name.startswith(".")
    ]
    return sorted(paths)


def resize_rgba_smooth(im: Image.Image, size: tuple[int, int]) -> Image.Image:
    """Resize RGB and alpha separately to reduce dark/bright halos on transparency edges."""
    r, g, b, a = im.split()
    return Image.merge(
        "RGBA",
        (
            r.resize(size, _LANCZOS),
            g.resize(size, _LANCZOS),
            b.resize(size, _LANCZOS),
            a.resize(size, _LANCZOS),
        ),
    )


def back_front_split(w: int) -> set[tuple[int, int]]:
    """Back = first half of cells in draw order (small row+col first) — iso depth away from viewer."""
    coords = [(r, c) for r in range(w) for c in range(w)]
    coords.sort(key=lambda rc: (rc[0] + rc[1], rc[0]))
    return set(coords[: len(coords) // 2])


def sample_stack_height(is_back: bool, grid_width: int) -> int:
    """Back +1 vs front on average. W=3: back 1–3 (mean 2), front 0–2 (mean 1). W=4: back 3–6, front 2–5."""
    if grid_width == 3:
        if is_back:
            return random.randint(1, 3)
        return random.randint(0, 2)
    if is_back:
        return random.randint(3, 6)
    return random.randint(2, 5)


def sample_heights_2x2_column_avg1() -> list[list[int]]:
    """Each grid column sums to 2 ⇒ average 1 per column; each cell in 0..2."""
    pairs = [(0, 2), (1, 1), (2, 0)]
    c0 = random.choice(pairs)
    c1 = random.choice(pairs)
    return [
        [c0[0], c1[0]],
        [c0[1], c1[1]],
    ]


def metrics_for(im: Image.Image) -> tuple[int, int, float, float, int]:
    """width, height, anchor_x, anchor_y (ground), per-layer stack step for this sprite."""
    w, h = im.size
    alpha = im.split()[3]
    bbox = alpha.getbbox()
    if not bbox:
        return w, h, w / 2.0, float(h), 28
    ax0, ay0, ax1, ay1 = bbox
    anchor_x = (ax0 + ax1) / 2.0
    anchor_y = float(ay1 - 10)
    stack_dy = max(28, int((ay1 - ay0) * 0.22))
    return w, h, anchor_x, anchor_y, stack_dy


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Build a transparent PNG city cluster from img/houses (WxW iso grid)."
    )
    parser.add_argument(
        "width",
        type=int,
        nargs="?",
        default=4,
        choices=(2, 3, 4),
        metavar="WIDTH",
        help="City width W for a WxW grid (2–4). W=2: column mean 1, cells 0–2. W=3: back 1–3 (mean 2), front 0–2 (mean 1).",
    )
    args = parser.parse_args()
    width: int = args.width

    out_path = ROOT / "img" / f"city_{width}x{width}_cluster.png"

    house_paths = list_house_paths()
    if not house_paths:
        house_paths = [FALLBACK]
        print("No PNGs in img/houses; using", FALLBACK.name)

    if (seed_env := os.environ.get("CITY_CLUSTER_SEED")) is not None:
        random.seed(int(seed_env))
    else:
        random.seed()

    images: dict[Path, Image.Image] = {}
    meta: dict[Path, tuple[int, int, float, float, int]] = {}
    ss = SUPERSAMPLE
    for p in house_paths:
        im = Image.open(p).convert("RGBA")
        if ss != 1:
            w0, h0 = im.size
            im = im.resize((w0 * ss, h0 * ss), Image.Resampling.NEAREST)
        images[p] = im
        meta[p] = metrics_for(im)

    stack_dy = max(m[4] for m in meta.values())

    if width == 2:
        heights = sample_heights_2x2_column_avg1()
        back_cells: set[tuple[int, int]] = set()
    else:
        back_cells = back_front_split(width)
        heights = [
            [sample_stack_height((r, c) in back_cells, width) for c in range(width)]
            for r in range(width)
        ]

    stacks: list[list[Path]] = []
    for row in range(width):
        for col in range(width):
            n = heights[row][col]
            stacks.append([random.choice(house_paths) for _ in range(n)])

    dx, dy = 88.0 * ss, 44.0 * ss
    origin_x, origin_y = 14.0 * ss, -9.0 * ss
    cells: list[tuple[float, float, list[Path], int, int]] = []
    idx = 0
    for row in range(width):
        for col in range(width):
            bx = (col - row) * dx + origin_x
            by = (col + row) * dy + origin_y
            cells.append((bx, by, stacks[idx], row, col))
            idx += 1

    margin = 100.0 * ss
    xs: list[float] = []
    ys: list[float] = []
    for bx, by, paths, _, _ in cells:
        for k, p in enumerate(paths):
            im = images[p]
            w, h, ax, ay, _ = meta[p]
            px = bx - ax
            py = by - ay - k * stack_dy
            xs += [px, px + w]
            ys += [py, py + h]
    if not xs:
        min_x, max_x = 0.0, 100.0 * ss
        min_y, max_y = 0.0, 100.0 * ss
    else:
        min_x, max_x = min(xs) - margin, max(xs) + margin
        min_y, max_y = min(ys) - margin, max(ys) + margin
    cw = int(max_x - min_x) + 1
    ch = int(max_y - min_y) + 1
    canvas_ox = -min_x
    canvas_oy = -min_y

    cells.sort(key=lambda t: (t[3] + t[4], t[3]))

    canvas = Image.new("RGBA", (cw, ch), (0, 0, 0, 0))
    for bx, by, paths, _, _ in cells:
        for k, p in enumerate(paths):
            im = images[p]
            _, _, ax, ay, _ = meta[p]
            x = int(round(canvas_ox + bx - ax))
            y = int(round(canvas_oy + by - ay - k * stack_dy))
            canvas.alpha_composite(im, (x, y))

    tight = canvas.getbbox()
    if tight:
        canvas = canvas.crop(tight)
    pad = 72 * ss
    w0, h0 = canvas.size
    padded = Image.new("RGBA", (w0 + 2 * pad, h0 + 2 * pad), (0, 0, 0, 0))
    padded.paste(canvas, (pad, pad))
    canvas = padded

    if ss > 1:
        w1, h1 = canvas.size
        nw = max(1, round(w1 / ss))
        nh = max(1, round(h1 / ss))
        canvas = resize_rgba_smooth(canvas, (nw, nh))

    canvas.save(out_path, "PNG")

    extra = ""
    if width == 2:
        col_sums = [sum(heights[r][c] for r in range(2)) for c in range(2)]
        col_means = [s / 2.0 for s in col_sums]
        extra = f" col_sums={col_sums} col_means={col_means}"
    else:
        back_h = [heights[r][c] for r, c in back_cells]
        front_h = [
            heights[r][c]
            for r in range(width)
            for c in range(width)
            if (r, c) not in back_cells
        ]
        mb = sum(back_h) / len(back_h)
        mf = sum(front_h) / len(front_h)
        extra = f" mean back={mb:.2f} front={mf:.2f} (Δ={mb - mf:.2f})"

    print(
        f"Wrote {out_path} ({canvas.size[0]}x{canvas.size[1]}) "
        f"width={width} heights={heights}{extra} "
        f"stack_dy={stack_dy // ss if ss else stack_dy} (1× layout units)"
    )


if __name__ == "__main__":
    main()
