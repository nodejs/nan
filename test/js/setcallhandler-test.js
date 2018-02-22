/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'setcallhandler' });

test('setcallhandler', function (t) {
  t.plan(4);
  t.type(bindings.a, 'function');
  t.type(bindings.b, 'function');
  t.equal(bindings.a()(), 12);
  t.equal(bindings.b()(), 15);
});
