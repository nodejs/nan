/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'asyncprogressworkersignal' });

test('asyncprogressworkersignal', function (t) {
  var worker = bindings.a
    , progressed = 0
  worker(100, 5, function(i) {
    t.ok(i === true, 'data being left at NULL');
    progressed++;
  }, function () {
    t.ok(progressed === 5, 'got all progress updates')
    t.end()
  })
})
