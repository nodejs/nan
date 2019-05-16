/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2019 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'maybe' })

test('maybe', function (t) {
    t.plan(1);
    t.ok(bindings.test());
});
