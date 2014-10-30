const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'nan_sketch' });

test('nan2', function (t) {
  t.plan(12);

  t.type(bindings.newIntegerWithValue, 'function');
  t.equals(bindings.newIntegerWithValue(23), 23);
  t.equals(bindings.newIntegerWithValue(5), 5);

  t.type(bindings.newNumberWithValue, 'function');
  t.equals(bindings.newNumberWithValue(Math.PI), Math.PI);
  t.equals(bindings.newNumberWithValue(Math.E), Math.E);

  t.type(bindings.newUint32WithValue, 'function');
  t.equals(bindings.newUint32WithValue(23), 23);
  t.equals(bindings.newUint32WithValue(5), 5);

  t.equals(bindings.newStringFromChars(), "hello?");
  t.equals(bindings.newStringFromCharsWithLength(), "hell");
  t.equals(bindings.newStringFromStdString(), "hello!");
});

