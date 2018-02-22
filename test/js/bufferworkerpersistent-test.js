/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'bufferworkerpersistent' })
    , crypto   = require('crypto');

test('bufferworkerpersistent', function (t) {
  var worker = bindings.a
    , called = 0
    , buf    = crypto.randomBytes(256)
    , bufHex = buf.toString('hex')

  t.plan(7);

  t.type(worker, 'function')

  worker(200, buf, function (_buf) {
    t.ok(Buffer.isBuffer(_buf), 'is buffer arg')
    t.equal(_buf.toString('hex'), bufHex)
  })

  buf = null // unref
})
