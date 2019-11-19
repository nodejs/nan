/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2019 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'methodswithdata' })

test('SetMethod with data', function (t) {
    t.plan(1);
    t.ok(bindings.testWithData());
});

test('accessors with data', function (t) {
    t.plan(7)
    var settergetter = bindings.create()
    t.equal(settergetter.prop1, 'this is property 1')
    t.ok(settergetter.prop2 === '')
    settergetter.prop2 = 'setting a value'
    t.equal(settergetter.prop2, 'setting a value')
    t.equal(settergetter.log(),
      'New()\n' +
      'Prop1:GETTER(this is property 1)\n' +
      'Prop2:GETTER()\n' +
      'Prop2:SETTER(setting a value)\n' +
      'Prop2:GETTER(setting a value)\n'
    )
    var derived = Object.create(settergetter)
    t.equal(derived.prop1, 'this is property 1')
    derived.prop2 = 'setting a new value'
    t.equal(derived.prop2, 'setting a new value')
    t.equal(settergetter.prop2, 'setting a new value')
  })
  