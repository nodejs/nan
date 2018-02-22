/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'trycatch' });

test('returnemptystring', function (t) {
  t.plan(2);
  t.type(bindings.r, 'function');
  try {
    bindings.r();
  } catch (err) {
    t.equal(err, 'waaa');
  }
});
