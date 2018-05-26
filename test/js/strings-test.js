/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'strings' });


test('FromV8String', function (t) {
  t.plan(6);

  var a = bindings.returnUtf8String;
  var b = bindings.heapString;

  t.type(a, 'function');
  t.type(b, 'function');

  t.equal(a('an utf8 strïng'), 'an utf8 strïng');
  t.equal(b('an utf8 strïng'), 'an utf8 strïng');

  var buf;

  /* we check Buffer.alloc rather than Buffer.from because
   * we don't want the base class Uint8Array.from */
  if (typeof(Buffer.alloc) === "function") {
    buf = Buffer.from('hello');
  } else {
    buf = new Buffer('hello');
  }
  t.equal(bindings.encodeHex(), buf.toString('hex'));
  t.equal(bindings.encodeUCS2(), 'hello');
});
