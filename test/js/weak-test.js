/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'weak' });

test('weak', function (t) {
  t.plan(3);

  var weak = bindings;
  t.type(weak.hustle, 'function');

  function f() {
    var count = 0;
    weak.hustle(function () {
      t.ok(count++ < 2);
    });
  };

  f();

  // run weak callback, should not dispose
  gc();

  // run weak callback, should dispose
  gc();

  // do not run weak callback
  gc();
});
