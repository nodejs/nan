/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'indexedinterceptors' });

test('indexedinterceptors', function (t) {
  t.plan(5);
  var interceptor = bindings.create();
  t.equal(interceptor[2], 'bar');
  interceptor[0] = 'setting a value';
  t.equal(interceptor[0], 'setting a value');
  delete interceptor[0];
  t.equal(interceptor[0], 'goober');
  t.ok(Object.prototype.hasOwnProperty.call(interceptor, 1));
  t.equal(Object.keys(interceptor)[0], '42');
});
