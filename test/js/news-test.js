const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'news' });

test('news', function (t) {
  t.plan(44);
  t.type(bindings.newNumber, 'function');
  t.type(bindings.newPositiveInteger, 'function');
  t.type(bindings.newNegativeInteger, 'function');
  t.type(bindings.newInt32FromPositive, 'function');
  t.type(bindings.newInt32FromNegative, 'function');
  t.type(bindings.newUint32FromPositive, 'function');
  t.type(bindings.newUint32FromNegative, 'function');
  t.type(bindings.newUtf8String, 'function');
  t.type(bindings.newLatin1String, 'function');
  t.type(bindings.newUcs2String, 'function');
  t.type(bindings.newRegExp, 'function');
  t.type(bindings.newStringObject, 'function');
  t.type(bindings.newNumberObject, 'function');
  t.type(bindings.newBooleanObject, 'function');
  t.type(bindings.newExternal, 'function');
  t.type(bindings.newSignature, 'function');
  t.type(bindings.newScript, 'function');
  t.type(bindings.newScript2, 'function');
  t.type(bindings.compileScript, 'function');
  t.type(bindings.compileScript2, 'function');
  t.type(bindings.newDate, 'function');
  t.type(bindings.newArray, 'function');

  t.equal(bindings.newNumber(), 0.5);
  t.equal(bindings.newPositiveInteger(), 1);
  t.equal(bindings.newNegativeInteger(), -1);
  t.equal(bindings.newInt32FromPositive(), -1);
  t.equal(bindings.newInt32FromNegative(), -1);
  t.equal(bindings.newUint32FromPositive(), 0xFFFFFFFF);
  t.equal(bindings.newUint32FromNegative(), 0xFFFFFFFF);
  t.equal(bindings.newUtf8String(), 'strïng');
  t.equal(bindings.newLatin1String(), 'strïng');
  t.equal(bindings.newUcs2String(), 'strïng');
  t.deepEquals(bindings.newRegExp(), /foo/g);
  t.deepEquals(bindings.newStringObject(), new String("foo"));
  t.deepEquals(bindings.newNumberObject(), new Number(0.5));
  t.deepEquals(bindings.newBooleanObject(), new Boolean(true));
  t.equals(bindings.newExternal(), 'passed');
  t.equals(bindings.newSignature(), 'string');
  t.equals(bindings.newScript(), 6);
  t.equals(bindings.newScript2(), 6);
  t.equals(bindings.compileScript(), 6);
  t.equals(bindings.compileScript2(), 6);
  t.deepEquals(bindings.newDate(), new Date(1337));
  t.deepEquals(bindings.newArray(), []);
});
