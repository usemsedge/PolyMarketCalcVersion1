/**
 * Runs findBestMarketLayout wasm off the main thread.
 * Protocol: main posts { type: 'calculate', rows, cols, flatGrid, cityFlat, actionOrder, numCities, numActions }
 * Worker replies { type: 'ready' } | { type: 'initError', message } |
 *   { type: 'ok', result: Int32Array, bestMarketTotal: number } (buffer transferred) | { type: 'error', message }
 */
import createModule from './marketcalc.js';

let wasm = null;

createModule()
  .then((m) => {
    wasm = m;
    self.postMessage({ type: 'ready' });
  })
  .catch((e) => {
    self.postMessage({
      type: 'initError',
      message: e && e.message ? e.message : String(e),
    });
  });

self.onmessage = (ev) => {
  const msg = ev.data;
  if (!msg || msg.type !== 'calculate') return;
  if (!wasm) {
    self.postMessage({ type: 'error', message: 'WASM not initialized in worker.' });
    return;
  }

  const { rows, cols, flatGrid, cityFlat, actionOrder, numCities, numActions } = msg;

  try {
    const mapDataPtr = wasm._malloc(rows * cols * 4);
    const ccPtr = wasm._malloc(cityFlat.length * 4);
    const aoPtr = wasm._malloc(numActions * 4);
    const resultLayoutPtr = wasm._malloc(rows * cols * 4);
    try {
      new Int32Array(wasm.HEAP32.buffer, mapDataPtr, rows * cols).set(flatGrid);
      new Int32Array(wasm.HEAP32.buffer, ccPtr, cityFlat.length).set(cityFlat);
      new Int32Array(wasm.HEAP32.buffer, aoPtr, numActions).set(actionOrder);

      const bestMarketTotal = wasm._findBestMarketLayout_wasm(
        mapDataPtr,
        rows,
        cols,
        ccPtr,
        numCities,
        aoPtr,
        numActions,
        resultLayoutPtr,
      );

      const out = new Int32Array(rows * cols);
      out.set(new Int32Array(wasm.HEAP32.buffer, resultLayoutPtr, rows * cols));
      self.postMessage(
        { type: 'ok', result: out, bestMarketTotal },
        [out.buffer],
      );
    } finally {
      wasm._free(mapDataPtr);
      wasm._free(ccPtr);
      wasm._free(aoPtr);
      wasm._free(resultLayoutPtr);
    }
  } catch (e) {
    self.postMessage({
      type: 'error',
      message: e && e.message ? e.message : String(e),
    });
  }
};
