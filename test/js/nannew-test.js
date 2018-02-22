/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'nannew' });


// register c++ functions as tests...
for (var symbol in bindings) {
  if (typeof bindings[symbol] == 'function' && symbol.match(/^test.*/)) {
    test('C++: ' + symbol, bindings[symbol]);
  }
}

test('numbers', function (t) {
  t.plan(12);

  t.type(bindings.newIntegerWithValue, 'function');
  t.equals(bindings.newIntegerWithValue(23), 23);
  t.equals(bindings.newIntegerWithValue(5), 5);
  t.type(bindings.newIntegerWithValue(23), 'number');

  t.type(bindings.newNumberWithValue, 'function');
  t.equals(bindings.newNumberWithValue(Math.PI), Math.PI);
  t.equals(bindings.newNumberWithValue(Math.E), Math.E);
  t.type(bindings.newNumberWithValue(Math.PI), 'number');

  t.type(bindings.newUint32WithValue, 'function');
  t.equals(bindings.newUint32WithValue(23), 23);
  t.equals(bindings.newUint32WithValue(5), 5);
  t.type(bindings.newUint32WithValue(5), 'number');
  t.end();
});


test('strings', function (t) {
  t.plan(3);

  t.equals(bindings.newStringFromChars(), "hello?");
  t.equals(bindings.newStringFromCharsWithLength(), "hell");
  t.equals(bindings.newStringFromStdString(), "hello!");

  t.end();
});

test('test MakeMaybe(...)', function (t) {
  t.plan(1);
  t.ok(bindings.invokeMakeMaybe() - Math.PI < 10e-8);
  t.end();
});
