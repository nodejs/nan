/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const version = process.versions.node.split('.');
if (version[0] < 9) {
  process.exit(0);
}

try {
  require('async_hooks');
} catch (e) {
  process.exit(0);
}

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , delay = require('bindings')({ module_root: testRoot, bindings: 'asyncresource' }).delay
    , asyncHooks = require('async_hooks');

test('asyncresource', function (t) {
  t.plan(7);

  var resourceAsyncId;
  var originalExecutionAsyncId;
  var beforeCalled = false;
  var afterCalled = false;
  var destroyCalled = false;

  var hooks = asyncHooks.createHook({
    init: function(asyncId, type, triggerAsyncId, resource) {
      if (type === 'nan:test.DelayRequest') {
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
  delay(1000, function() {
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
