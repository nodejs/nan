/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'weak2' });

test('weak2', function (t) {
  t.plan(2);

  var weak2 = bindings;
  t.type(weak2.hustle, 'function');

  function f() {
    weak2.hustle(function (val) {
      t.equal(val, 42);
    });
  };

  f();

  // run weak callback, should dispose
  gc();

  // do not run weak callback
  gc();
});
