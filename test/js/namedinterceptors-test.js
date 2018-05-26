/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'namedinterceptors' });

test('namedinterceptors', function (t) {
  t.plan(5);
  var interceptor = bindings.create();
  t.equal(interceptor.prop, 'foo');
  interceptor.prop = 'setting a value';
  t.equal(interceptor.prop, 'setting a value');
  delete interceptor.something;
  t.equal(interceptor.prop, 'goober');
  t.ok(Object.prototype.hasOwnProperty.call(interceptor, "thing"));
  t.ok(Object.keys(interceptor)[0] === 'value');
});
