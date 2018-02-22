/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'buffer' });

test('buffer', function (t) {
  t.plan(8);
  t.type(bindings.new1, 'function');
  t.type(bindings.new2, 'function');
  t.type(bindings.new3, 'function');
  t.type(bindings.copy, 'function');
  t.equal(bindings.new1().toString(), 'abcdefghijklmnopqrstuvwxyz');
  t.equal(bindings.new2().toString(), 'abcdefghijklmnopqrstuvwxyz');
  t.equal(bindings.new3().toString(), 'abcdefghijklmnopqrstuvwxyz');
  t.equal(bindings.copy().toString(), 'abcdefghijklmnopqrstuvwxyz');
});
