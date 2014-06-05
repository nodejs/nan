const test     = require('tap').test
var EventEmitter = require('events').EventEmitter;
var MyObject = require('bindings')('makecallback').MyObject;

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
