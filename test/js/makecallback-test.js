/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , MyObject = require('bindings')({ module_root: testRoot, bindings: 'makecallback' }).MyObject
    , EventEmitter = require('events').EventEmitter;

// extend prototype
function inherits(target, source) {
    for (var k in source.prototype) {
        target.prototype[k] = source.prototype[k];
    }
}

inherits(MyObject, EventEmitter);

test('makecallback', function (t) {
    t.plan(1);

    var obj = new MyObject();
    obj.on('event', function() {
        t.ok(true);
    });

    obj.call_emit();
});
