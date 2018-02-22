/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'persistent' });

test('persistent', function (t) {
  t.plan(15);

  var persistent = bindings;
  t.type(persistent.save1, 'function');
  t.type(persistent.get1, 'function');
  t.type(persistent.dispose1, 'function');
  t.type(persistent.toPersistentAndBackAgain, 'function');
  t.type(persistent.persistentToPersistent, 'function');
  t.type(persistent.copyablePersistent, 'function');
  t.type(persistent.passGlobal, 'function');

  t.deepEqual(persistent.toPersistentAndBackAgain({ x: 42 }), { x: 42 });

  t.equal(persistent.persistentToPersistent('any string'), 'any string');

  persistent.save1('a string to save');
  t.equal(persistent.get1(), 'a string to save');
  t.equal(persistent.copyablePersistent(), 'a string to save');

  t.equal(persistent.passGlobal(), 42, 'pass global');

  setTimeout(function () {
    t.equal(persistent.get1(), 'a string to save');
    persistent.dispose1();
    t.ok(persistent.get1() === undefined, 'no more persistent');
    t.ok(persistent.copyablePersistent() === undefined, 'no more persistent');
  }, 25);
});
