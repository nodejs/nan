/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'strings' });


test('FromV8String', function (t) {
  t.plan(2);

  t.equal(bindings.encodeHex(), new Buffer('hello').toString('hex'));
  t.equal(bindings.encodeUCS2(), 'hello');
});
