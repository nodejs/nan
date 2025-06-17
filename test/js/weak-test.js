/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'weak' })
    , gc = require('./gc-fn');
const nodeVersion = parseInt(process.versions.node.split('.')[0]);

// https://github.com/nodejs/nan/issues/995
// https://chromium-review.googlesource.com/c/v8/v8/+/4733273
// Week tests are incompatible with recent V8 changes

if(nodeVersion < 24){
  test('weak', function (t) {
    t.plan(3);

    var weak = bindings, count = 0;
    t.type(weak.hustle, 'function');

    weak.hustle(function () {}, function (val) {
      t.equal(val, 42);
      count++;
    });

    var timeout = setTimeout(function () {
      // run weak callback, should dispose
      gc();

      // do not run weak callback
      gc();

      if (count > 0) {
        clearTimeout(timeout);
        t.equal(count, 1);
      }
    }, 100);
  });

  test('weak external', function (t) {
    t.plan(2);

    var weak = bindings;
    t.type(weak.weakExternal, 'function');

    weak.weakExternal();
    t.ok(true);  // All good if the previous line didn't crash.
  });
}

