/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'nancallback' })
    , round = Math.round;

test('nancallback', function (t) {
  t.plan(13)

  var persistent = bindings;
  t.type(persistent.globalContext, 'function');
  t.type(persistent.specificContext, 'function');
  t.type(persistent.customReceiver, 'function');
  t.type(persistent.compareCallbacks, 'function');
  t.type(persistent.callDirect, 'function');
  t.type(persistent.callAsFunction, 'function');
  persistent.globalContext(function () { t.ok(true); });
  persistent.specificContext(function () { t.ok(true); });
  persistent.customReceiver(function () { t.equal(this, process); }, process);
  persistent.callDirect(function () { t.ok(true); });
  persistent.callAsFunction(function () { t.ok(true); });

  var round2 = Math.round
    , x = function(param) { return param + 1; }
    , y = function(param) { return param + 2; }
    , x2 = x;
  t.ok(persistent.compareCallbacks(round, round2, Math.floor));
  t.ok(persistent.compareCallbacks(x, x2, y));
})
