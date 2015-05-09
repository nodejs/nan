/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'returnvalue' });

test('returnvalue', function (t) {
  t.plan(7);
  t.type(bindings.r, 'function');
  t.type(bindings.p, 'function');
  t.type(bindings.q, 'function');
  t.equal(bindings.r('a string value'), 'a string value');
  t.equal(bindings.r(), 'default');
  t.ok(bindings.p());
  t.ok(bindings.q());
});
