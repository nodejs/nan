/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'global' });

test('global', function (t) {
  t.plan(10);

  var global_ = bindings;
  t.type(global_.passGlobal, 'function');
  t.type(global_.emptyGlobal, 'function');
  t.type(global_.moveConstructGlobal, 'function');
  t.type(global_.copyConstructGlobal, 'function');
  t.type(global_.moveAssignGlobal, 'function');

  t.equal(global_.passGlobal(), 42, 'pass global');
  t.ok(global_.emptyGlobal());
  t.ok(global_.moveConstructGlobal());
  t.ok(global_.copyConstructGlobal());
  t.ok(global_.moveAssignGlobal());
});
