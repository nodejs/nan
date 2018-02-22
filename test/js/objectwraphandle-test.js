/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'objectwraphandle' });

test('objectwraphandle', function (t) {
  t.plan(9);

  t.type(bindings.MyObject, 'function');

  var obj = new bindings.MyObject(10);

  t.type(obj.getHandle, 'function');
  t.type(obj.getHandleConst, 'function');
  t.type(obj.getValue, 'function');
  t.type(obj.getHandle(), 'object');
  t.type(obj.getHandleConst(), 'object');
  t.type(obj.getValue(), 'number');

  var derived = Object.create(obj);
  t.type(derived, bindings.MyObject);
  try {
    // In Node 0.10 this call is valid:
    t.equal(derived.getValue(), 10);
  } catch (err) {
    // In Node >= 0.12 it throws instead:
    t.type(err, TypeError);
  }
});
