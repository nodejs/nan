const test     = require('tap').test
    , bindings = require('bindings');

test('fromv8string', function (t) {
  const ASCII  = 0,
        UTF8   = 1,
        BASE64 = 2,
        UCS2   = 3,
        BINARY = 4,
        HEX    = 5,
        BUFFER = 6;

  t.plan(8);
  t.type(bindings('fromv8string').r, 'function');
  t.equal(bindings('fromv8string').r('an ascii string', ASCII), 'an ascii string');
  t.equal(bindings('fromv8string').r('an utf8 strïng', UTF8), 'an utf8 strïng');
  t.equal(bindings('fromv8string').r('YSBiYXNlNjQgc3RyaW5n', BASE64), 'a base64 string');
  t.equal(bindings('fromv8string').r('an ucs2 strïng', UCS2), 'an ucs2 strïng');
  t.equal(bindings('fromv8string').r(new Buffer('a binary string'), BINARY), 'a binary string');
  t.equal(bindings('fromv8string').r('612068657820737472696E67', HEX), 'a hex string');
  t.equal(bindings('fromv8string').r(new Buffer('a buffer string'), BUFFER), 'a buffer string');
});
