const test     = require('tap').test
    , bindings = require('bindings');

test('morenews', function (t) {
  t.plan(16);
  t.type(bindings('morenews').newNumber, 'function');
  t.type(bindings('morenews').newPositiveInteger, 'function');
  t.type(bindings('morenews').newNegativeInteger, 'function');
  t.type(bindings('morenews').newUtf8String, 'function');
  t.type(bindings('morenews').newLatin1String, 'function');
  t.type(bindings('morenews').newUcs2String, 'function');
  t.type(bindings('morenews').newExternalStringResource, 'function');
  t.type(bindings('morenews').newExternalAsciiStringResource, 'function');

  t.equal(bindings('morenews').newNumber(), 0.5);
  t.equal(bindings('morenews').newPositiveInteger(), 1);
  t.equal(bindings('morenews').newNegativeInteger(), -1);
  t.equal(bindings('morenews').newUtf8String(), 'strïng');
  t.equal(bindings('morenews').newLatin1String(), 'strïng');
  t.equal(bindings('morenews').newUcs2String(), 'strïng');
  t.equals(bindings('morenews').newExternalStringResource(), 'strïng');
  t.equals(bindings('morenews').newExternalAsciiStringResource(), 'string');
});
