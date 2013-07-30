const test     = require('tap').test
    , bindings = require('bindings')

test('symbols', function (t) {
  t.plan(2)
  t.type(bindings('symbols').key, 'string')
  t.equal(bindings('symbols').key, 'a property')
})