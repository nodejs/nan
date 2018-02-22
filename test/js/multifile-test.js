/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'multifile' });

// same as fromv8string test but split over multiple compile units
// just to test that there aren't any conflicts from including nan.h
// more than once in project files
test('multifile', function (t) {
  t.plan(2);

  var r = bindings.r
  t.type(r, 'function');
  t.equal(r('a string value'), 'a string value');
});
