/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'weak' });

test('weak', function (t) {
  t.plan(3);

  var weak = bindings, count = 0;
  t.type(weak.hustle, 'function');

  weak.hustle(function () {}, function (val) {
    t.equal(val, 42);
    count++;
  });

  // run weak callback, should dispose
  gc();

  // do not run weak callback
  gc();

  t.equal(count, 1);
});
