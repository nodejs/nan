/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'error' });

const errors = ['Error', 'RangeError', 'ReferenceError', 'SyntaxError', 'TypeError'];

test('error', function (t) {
  t.plan(4 * errors.length * 3);
  errors.forEach(function (val) {
    var i;
    for (i = 1; i <= 4; i++) {
      t.type(bindings['Throw' + val + i], 'function');
      try {
        bindings['Throw' + val + i]();
      } catch (err) {
        t.type(err, val);
        t.equal(err.message, 'errmsg');
      }
    }
  });
});
