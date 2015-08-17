/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'objectwraphandle' });

test('objectwraphandle', function (t) {
  t.plan(5);

  t.type(bindings.MyObject, 'function');

  var obj = new bindings.MyObject(10);

  t.type(obj.getHandle, 'function');
  t.type(obj.getValue, 'function');
  t.type(obj.getHandle(), 'object');
  t.type(obj.getValue(), 'number');
});
