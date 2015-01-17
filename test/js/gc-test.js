/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'gc' });

test('gc', function (t) {
  t.plan(3);

  t.type(bindings.hook, 'function');

  bindings.hook(function (from) {
    t.ok(from == 'prologue' || from == 'epilogue');
  });

  gc();
});
