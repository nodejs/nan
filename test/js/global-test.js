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
  t.plan(28);

  var global_ = bindings;
  t.type(global_.passGlobal, 'function');
  t.type(global_.emptyGlobal, 'function');
  t.type(global_.assignInitGlobalFromTemporary, 'function');
  t.type(global_.assignInitGlobalFromLVal, 'function');
  t.type(global_.assignGlobalFromTemporary, 'function');
  t.type(global_.assignGlobalFromLVal, 'function');
  t.type(global_.constructGlobalFromNonConstGlobalTemporary, 'function');
  t.type(global_.constructGlobalFromNonConstGlobalLVal, 'function');
  t.type(global_.constructGlobalFromConstGlobalTemporary, 'function');
  t.type(global_.constructGlobalFromConstGlobalLVal, 'function');
  t.type(global_.constructGlobalFromNonConstPersistentBaseTemporary, 'function');
  t.type(global_.constructGlobalFromNonConstPersistentBaseLVal, 'function');
  t.type(global_.constructGlobalFromConstPersistentBaseTemporary, 'function');
  t.type(global_.constructGlobalFromConstPersistentBaseLVal, 'function');

  t.equal(global_.passGlobal(), 42, 'pass global');
  t.ok(global_.emptyGlobal());
  t.ok(global_.assignInitGlobalFromTemporary());
  t.ok(global_.assignInitGlobalFromLVal());
  t.ok(global_.assignGlobalFromTemporary());
  t.ok(global_.assignGlobalFromLVal());
  t.ok(global_.constructGlobalFromNonConstGlobalTemporary());
  t.ok(global_.constructGlobalFromNonConstGlobalLVal());
  t.ok(global_.constructGlobalFromConstGlobalTemporary());
  t.ok(global_.constructGlobalFromConstGlobalLVal());
  t.ok(global_.constructGlobalFromNonConstPersistentBaseTemporary());
  t.ok(global_.constructGlobalFromNonConstPersistentBaseLVal());
  t.ok(global_.constructGlobalFromConstPersistentBaseTemporary());
  t.ok(global_.constructGlobalFromConstPersistentBaseLVal());
});
