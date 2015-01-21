/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')(
        { module_root: testRoot, bindings: 'nannew_function_length' });


// register c++ functions as tests...
for (var symbol in bindings) {
  if (typeof bindings[symbol] == 'function' && symbol.match(/^test.*/)) {
    test('C++: ' + symbol, bindings[symbol]);
  }
}

test('function length', function (t) {
  var skip = {skip: process.versions.modules < 12};
  t.plan(2);
  t.equals(bindings.lengthFiveFunction().length, 5, undefined, skip);
  t.equals(bindings.lengthSevenFunctionFromTemplate().length, 7, undefined, skip);
  t.end();
});
