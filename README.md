# PolyMarketCalc


What you can do as a user:

- Calculate the best market spots for either windmills or sawmills, but not both
  - Tell you which BGs are best?
- Simulate capturing cities, border growths, and placing buildings/resources
- Assumes every unused resource is either used or has a building/market placed on it



Optimizations:
Debug process:

Initially
Requirement to be served through the browser
Found a 3-city solution online that took 30 seconds
Realized native JS would be way too slow, must use lower level
  Settled on C++ compiled to WASM for calculations, basic JS for frontend

First prototype
Vibe coded and iterated through the UI, it just needs to be attached to a serializable format
4 cities took 1 second (so with BG iteration it takes 24 seconds)
Thought it was too slow, looked through profiler and found hashing was expensive

Second prototype
Debugging is a pain

PROBLEM: I tried to save space by having a bestlayoutcurrent and a bestlayoutreturn.
Only those two data structures for storing intermediate layouts during calculation.
However, you need an additional bestlayoutcurrent for every recursion depth because
they will eat into each other, and I failed to consider that and spent 2 hours deb:w
ugging
for this issue.:w



recursive backtracking to find the best possible arrangement of stuff
(best place to place a market in EACH city)
each recursion depth places the market in 1 city
  then calls recursion depth + 1 and finds the best layout for each 