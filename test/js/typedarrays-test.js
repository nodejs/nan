/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'typedarrays' });

test('typedarrays - simple cases', function (t) {
  if (typeof Uint8Array !== 'function') {

    t.pass('typedarrays not supported');
    t.end();

  } else {

    var zeros = new Uint8Array(5)
    t.same(bindings.ReadU8(zeros), [0,0,0,0,0])

    var y = zeros[0]
    t.equals(y, 0)
    t.same(bindings.ReadU8(zeros), [0,0,0,0,0])

    var u8array = new Uint8Array([1, 255, 3]);
    t.same(bindings.ReadU8(u8array), [1, 255, 3]);
    t.same(bindings.ReadU8(u8array.subarray(1)), [255, 3]);
    t.same(bindings.ReadU8(u8array.subarray(0, 2)), [1, 255]);
    t.same(bindings.ReadU8(u8array.subarray(1, 2)), [255]);

    t.same(bindings.ReadU8(new Uint8Array(u8array)), [1, 255, 3]);
    t.same(bindings.ReadU8(new Uint8Array(u8array.subarray(1))), [255, 3]);
    t.same(bindings.ReadU8(new Uint8Array(u8array.subarray(0, 2))), [1, 255]);
    t.same(bindings.ReadU8(new Uint8Array(u8array.subarray(1, 2))), [255]);

    t.same(bindings.ReadU8((new Uint8Array(u8array.buffer)).subarray(1)), [255, 3]);


    var i32array = new Int32Array([0, 1, -1, 1073741824, -1073741824]);
    t.same(bindings.ReadI32(i32array), [0, 1, -1, 1073741824, -1073741824]);

    var f32array = new Float32Array([1, -1, Infinity, -Infinity, 0, +0, -0]);
    t.same(bindings.ReadFloat(f32array), [1, -1, Infinity, -Infinity, 0, +0, -0]);
    t.same(bindings.ReadFloat(f32array.subarray(1)), [-1, Infinity, -Infinity, 0, +0, -0]);
    t.same(bindings.ReadFloat(f32array.subarray(0,4)), [1, -1, Infinity, -Infinity]);
    t.same(bindings.ReadFloat(f32array.subarray(1,3)), [-1, Infinity]);

    t.end();
  }
});

test('typedarrays - bad arguments', function (t) {
  if (typeof Uint8Array !== 'function') {

    t.pass('typedarrays not supported');
    t.end();

  } else {

    t.same(bindings.ReadU8(0), []);
    t.same(bindings.ReadU8(1), []);
    t.same(bindings.ReadU8(null), []);
    t.same(bindings.ReadU8(), []);
    t.same(bindings.ReadU8('foobar'), []);
    t.same(bindings.ReadU8([]), []);
    t.same(bindings.ReadU8([1,2]), []);
    t.same(bindings.ReadU8({}), []);
    t.same(bindings.ReadU8(Uint8Array), []);
    t.same(bindings.ReadU8(new Float32Array(0)), []);

    t.same(bindings.ReadU8({
      byteLength: 10000000,
      byteOffset: 100000,
      buffer: null
    }), [])

    t.end();
  }
});

test('typed arrays - Nan::New<[typedarray]>', function (t) {
  const types = [
    {fn: "NewFloat32Array", ctor: Float32Array},
    {fn: "NewFloat64Array", ctor: Float64Array},
    {fn: "NewInt8Array", ctor: Int8Array},
    {fn: "NewUint8Array", ctor: Uint8Array},
    {fn: "NewUint8ClampedArray", ctor: Uint8ClampedArray},
    {fn: "NewInt16Array", ctor: Int16Array},
    {fn: "NewUint16Array", ctor: Uint16Array},
    {fn: "NewInt32Array", ctor: Int32Array},
    {fn: "NewUint32Array", ctor: Uint32Array}
  ];

  t.plan(4 * types.length);
  types.forEach(function (type) {
    t.type(bindings[type.fn], 'function');
    t.same(bindings[type.fn](), new type.ctor([0, 0, 0, 0]));

    // TODO:
    var fname = type.fn + 'FromArrayBuffer';
    t.type(bindings[fname], 'function');
    t.same(bindings[fname](), new type.ctor([0, 0, 0, 0]));
  });
  t.end();
});
