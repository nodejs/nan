/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const version = process.versions.node.split('.');
const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'asyncworker' });

test('asyncworker', function (t) {
  var worker = bindings.a
    , ticks  = 0
    , called = false
  t.type(worker, 'function')
  function tick () {
    ticks++
    if (!called)
      setTimeout(tick, 0)
  }
  setTimeout(tick, 0)
  worker(200, function () {
    called = true
    t.ok(ticks > 6, 'got plenty of ticks! (' + ticks + ')')
    t.end()
  })
});

test('asyncworker context', function (t) {
  var asyncHooks;
  if (version[0] < 9) {
    t.ok(true);
    t.end();
    return;
  }

  try {
    asyncHooks = require('async_hooks');
  } catch (e) {
    t.ok(true);
    t.end();
    return;
  }

  t.plan(7);

  var sleep = bindings.a;
  var resourceAsyncId;
  var originalExecutionAsyncId;
  var beforeCalled = false;
  var afterCalled = false;
  var destroyCalled = false;

  var hooks = asyncHooks.createHook({
    init: function(asyncId, type, triggerAsyncId, resource) {
      if (type === 'nan:test.SleepWorker') {
        resourceAsyncId = asyncId;
      }
    },
    before: function(asyncId) {
      if (asyncId === resourceAsyncId) {
        beforeCalled = true;
      }
    },
    after: function(asyncId) {
      if (asyncId === resourceAsyncId) {
        afterCalled = true;
      }
    },
    destroy: function(asyncId) {
      if (asyncId === resourceAsyncId) {
        destroyCalled = true;
      }
    }

  });
  hooks.enable();

  originalExecutionAsyncId = asyncHooks.executionAsyncId();
  sleep(200, function() {
    t.equal(asyncHooks.executionAsyncId(), resourceAsyncId,
            'callback should have the correct execution context');
    t.equal(asyncHooks.triggerAsyncId(), originalExecutionAsyncId,
            'callback should have the correct trigger context');
    t.ok(beforeCalled, 'before should have been called');
    t.notOk(afterCalled, 'after should not have been called yet');
    setTimeout(function() {
      t.ok(afterCalled, 'after should have been called');
      t.ok(destroyCalled, 'destroy should have been called');
      t.equal(asyncHooks.triggerAsyncId(), resourceAsyncId,
              'setTimeout should have been triggered by the async resource');
      hooks.disable();
    }, 1);
  });
});

