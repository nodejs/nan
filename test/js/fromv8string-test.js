const test     = require('tap').test
    , bindings = require('bindings')

test('fromv8string', function (t) {
  const ASCII  = 0,
        UTF8   = 1,
        BASE64 = 2,
        UCS2   = 3,
        BINARY = 4,
        HEX    = 5,
        BUFFER = 6;

  t.plan(3)
  t.type(bindings('fromv8string').r, 'function')
  t.equal(bindings('fromv8string').r('a string value'), 'a string value')
  t.equal(bindings('fromv8string').r('612068657820737472696E67', HEX), 'a hex string')
})
