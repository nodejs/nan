const test     = require('tap').test
    , bindings = require('bindings')

test('fromv8string', function (t) {
  t.plan(2)
  t.type(bindings('fromv8string').r, 'function')
  t.equal(bindings('fromv8string').r('a string value'), 'a string value')
})