/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'asyncprogressqueueworkerstream' })
    , util = require('util');

const nodeVersion = process.versions.node.split('.')
var Readable
if (nodeVersion[0] == 0 && nodeVersion[1] <= 8)
  Readable = require('readable-stream')
else
  Readable = require('stream').Readable

function StreamProgressWorker(t) {
  Readable.call(this, {objectMode: true})
  var self = this
  // initialize stream from cpp on next tick
  process.nextTick(function () {
    var worker = bindings.doProgress
    worker(5, function(i) {
      self.push(i)
    }, function () {
      self.push(null)
    })
  })
}
util.inherits(StreamProgressWorker, Readable)

StreamProgressWorker.prototype._read = function (data) {

};


test('asyncprogressqueueworker', function (t) {
  var stream = new StreamProgressWorker(t)
  var progressed = 0;

  stream
    .on('end', function() {
      t.ok(progressed === 5, 'cpp should have sent 5 objects')
      t.end()
    })
    .on('data', function(data) {
      progressed++
      console.log(data);
    })
})
