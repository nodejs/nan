const test     = require('tap').test
    , bindings = require('bindings')

test('settergetter', function (t) {
  t.plan(4)
  var settergetter = bindings('settergetter').create()
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
})
