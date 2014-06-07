const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'strings' })
    , crypto   = require('crypto');

const ASCII  = 0
    , UTF8   = 1
    , BASE64 = 2
    , UCS2   = 3
    , BINARY = 4
    , HEX    = 5
    , BUFFER = 6

test('FromV8String', function (t) {
  t.plan(17)

  var r = bindings.returnString

  t.type(r, 'function')
  t.equal(r('an ascii string', ASCII), 'an ascii string')
  t.equal(r('an utf8 strïng', UTF8), 'an utf8 strïng')
  t.equal(r('YSBiYXNlNjQgc3RyaW5n', BASE64), 'a base64 string')
  t.equal(r('an ucs2 strïng', UCS2), 'an ucs2 strïng')
  t.equal(r('a binary string', BINARY), 'a binary string')
  t.equal(r('612068657820737472696E67', HEX), 'a hex string')
  t.equal(r('a buffer string', BUFFER), 'a buffer string')
  t.equal(r(new Buffer('an actual buffer'), BUFFER), 'an actual buffer')
  t.equal(r('an ascii string', ASCII, 1), 'an ascii string')
  t.equal(r('an utf8 strïng', UTF8, 1), 'an utf8 strïng')
  t.equal(r('YSBiYXNlNjQgc3RyaW5n', BASE64, 1), 'a base64 string')
  t.equal(r('an ucs2 strïng', UCS2, 1), 'an ucs2 strïng')
  t.equal(r('a binary string', BINARY, 1), 'a binary string')
  t.equal(r('612068657820737472696E67', HEX, 1), 'a hex string')
  t.equal(r('a buffer string', BUFFER, 1), 'a buffer string')
  t.equal(r(new Buffer('an actual buffer'), BUFFER, 1), 'an actual buffer')
})

test('return NanCString', function (t) {
  t.plan(2);
  var r = bindings.returnCString
  t.type(r, 'function');
  t.equal(r('an utf8 strïng'), 'an utf8 strïng');
});

test('compare NanCString', function (t) {
  var compareCStringToBuffer = bindings.compareCStringToBuffer
  t.type(compareCStringToBuffer, 'function')

  try {
    t.ok(compareCStringToBuffer('this is a standard c string', new Buffer('this is a standard c string')))
  } catch (e) {
    t.fail(e)
  }
  t.end()
})

test('compare NanRawString', function (t) {
  var compareRawStringToBuffer = bindings.compareRawStringToBuffer
    , rndStr                   = crypto.randomBytes(32)

  t.type(compareRawStringToBuffer, 'function')

  try {
    t.ok(compareRawStringToBuffer(rndStr.toString('base64'), rndStr))
  } catch (e) {
    t.fail(e)
  }
  t.end()
})
