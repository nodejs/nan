/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'wrappedobjectfactory' });

test('wrappedobjectfactory', function (t) {
  t.plan(4);

  var obj = bindings.newFactoryObjectInstance(123);

  t.type(obj.getValue, 'function');
  t.type(obj.getValue(), 'number');
  t.equal(obj.getValue(), 123);

  var inner = obj.newInnerObject(456);
  t.equal(inner.getValue(), 456);
});
