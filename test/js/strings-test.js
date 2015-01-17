/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'strings' });


test('FromV8String', function (t) {
  t.plan(8);

  var a = bindings.returnAsciiString;
  var b = bindings.returnUtf8String;
  var c = bindings.returnUcs2String;
  var d = bindings.heapString;

  t.type(a, 'function');
  t.type(b, 'function');
  t.type(c, 'function');
  t.type(d, 'function');

  t.equal(a('an ascii string'), 'an ascii string');
  t.equal(b('an utf8 strïng'), 'an utf8 strïng');
  t.equal(c('an ucs2 strïng'), 'an ucs2 strïng');
  t.equal(d('an utf8 strïng'), 'an utf8 strïng');
});
