/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'wrappedobjectfactory' });

test('objectwraphandle', function (t) {
  t.plan(3);

  var obj = bindings.newFactoryObjectInstance(123);

  t.type(obj.getValue, 'function');
  t.type(obj.getValue(), 'number');
  t.equal(obj.getValue(), 123);
});
