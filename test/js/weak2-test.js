/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'weak2' })
    , gc = require('./gc-fn');

test('weak2', function (t) {
  t.plan(3);

  var weak2 = bindings, count = 0;
  t.type(weak2.hustle, 'function');

  weak2.hustle(function (val) {
    t.equal(val, 42);
    count++;
  });

  // run weak callback, should dispose
  gc();

  // do not run weak callback
  gc();

  var timeout = setTimeout(function () {
    if (count > 0) {
      clearTimeout(timeout);
      t.equal(count, 1);
    }
  }, 100);
});
