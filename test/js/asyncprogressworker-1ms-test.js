/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2017 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'asyncprogressworker' });

test('asyncprogressworker1ms', function (t) {
  // test with 1 ms sleep
  var worker = bindings.a
    , progressed = 0
  worker(1, 500, function(i) {
    t.ok(i >= progressed, 'got the progress updates #' + i);
    progressed++;
  }, function () {
    t.ok(progressed <= 500, 'got some but not necessarily all progress updates')
    t.end()
  })
})
