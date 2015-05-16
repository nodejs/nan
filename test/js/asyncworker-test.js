/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'asyncworker' });

test('asyncworker', function (t) {
  var worker = bindings.a
    , ticks  = 0
    , called = false
  t.type(worker, 'function')
  function tick () {
    ticks++
    if (!called)
      setTimeout(tick, 0)
  }
  setTimeout(tick, 0)
  worker(200, function () {
    called = true
    t.ok(ticks > 6, 'got plenty of ticks! (' + ticks + ')')
    t.end()
  })
})
