/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'bufferworkerpersistent' })
    , crypto   = require('crypto');

test('bufferworkerpersistent', function (t) {
  var worker = bindings.a
    , called = false
    , buf    = crypto.randomBytes(256)
    , bufHex = buf.toString('hex')

  t.type(worker, 'function')

  setTimeout(function () {
    t.ok(called, 'callback fired')
    t.end()
  }, 500)

  worker(200, buf, function (_buf) {
    called = true
    t.ok(Buffer.isBuffer(_buf), 'is buffer arg')
    t.equal(_buf.toString('hex'), bufHex)
  })

  buf = null // unref
})
