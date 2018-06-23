// Run the tap tests in a worker
// Not a clean approach, but is sufficient to verify correctness
const worker_threads = require("worker_threads");
const path = require("path");

if (worker_threads.isMainThread) {
	const worker = new worker_threads.Worker(__filename, { workerData: process.argv });
	worker.on("error", console.error);
} else {
	process.argv = worker_threads.workerData;
	require(path.resolve(require.resolve("tap"), "../../bin/tap.js"));
}
