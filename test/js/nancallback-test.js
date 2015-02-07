/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'nancallback' });

test('nancallback', function (t) {
  t.plan(4)

  var persistent = bindings;
  t.type(persistent.globalContext, 'function');
  t.type(persistent.specificContext, 'function');
  persistent.globalContext(function () { t.ok(true); });
  persistent.specificContext(function () { t.ok(true); });
})
