/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2026 NAN contributors
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

const test = require('tap').test
  , testRoot = require('path').resolve(__dirname, '..')
  , delay = require('bindings')({ module_root: testRoot, bindings: 'asyncresource-promise' }).delay
  , asyncHooks = require('async_hooks');

test('asyncresource-promise', function (t) {
  t.plan(2);

  var beforeCalled = false;
  var promiseResolveCalled = false;

  var hooks = asyncHooks.createHook({
    init: function (asyncId, type, triggerAsyncId, resource) {
      if (type === 'nan:test.DelayPromise') {
        resourceAsyncId = asyncId;
      }
    },
    before: function (asyncId) {
      if (asyncId === resourceAsyncId) {
        beforeCalled = true;
      }
    },
    promiseResolve: function (asyncId) {
      promiseResolveCalled = true;
    }

  });
  hooks.enable();

  originalExecutionAsyncId = asyncHooks.executionAsyncId();
  delay(1000).then(function() {
    t.ok(beforeCalled, 'before should have been called');
    t.ok(promiseResolveCalled, 'promiseResolve should have been called');
  })
});
