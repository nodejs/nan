/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'returnundefined' });

test('returnundefined', function (t) {
  t.plan(3);
  t.type(bindings.r, 'function');
  t.equal(bindings.r('a string value'), undefined);
  t.equal(bindings.r(), undefined);
});
