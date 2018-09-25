/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

#include <stdint.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(ReadU8) {
  TypedArrayContents<uint8_t> data(info[0]);

  v8::Local<v8::Array> result = New<v8::Array>(data.length());
  for (size_t i=0; i < data.length(); i++) {
    Set(result, i, New<v8::Number>((*data)[i]));
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(ReadI32) {
  TypedArrayContents<int32_t> data(info[0]);

  v8::Local<v8::Array> result = New<v8::Array>(data.length());
  for (size_t i=0; i < data.length(); i++) {
    Set(result, i, New<v8::Number>((*data)[i]));
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(ReadFloat) {
  TypedArrayContents<float> data(info[0]);

  v8::Local<v8::Array> result = New<v8::Array>(data.length());\
  for (size_t i=0; i < data.length(); i++) {
    Set(result, i, New<v8::Number>((*data)[i]));
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(ReadDouble) {
  TypedArrayContents<double> data(info[0]);

  v8::Local<v8::Array> result = New<v8::Array>(data.length());
  for (size_t i=0; i < data.length(); i++) {
    Set(result, i, New<v8::Number>((*data)[i]));
  }

  info.GetReturnValue().Set(result);
}

#define NEW_TA(ENAME, T)                                                      \
  NAN_METHOD(ENAME) {                                                         \
    v8::Local<T> ta = New<T>(4);                                              \
    info.GetReturnValue().Set(ta);                                            \
  }                                                                           \
  NAN_METHOD(ENAME##FromArrayBuffer) {                                        \
    size_t byteLength = 4 * sizeof(TypedArrayCType<T>::type);                 \
    v8::Local<v8::ArrayBuffer> buffer = v8::ArrayBuffer::New(                 \
      v8::Isolate::GetCurrent(), byteLength);                                 \
    v8::Local<T> ta = New<T>(buffer, 0, 4);                                   \
    info.GetReturnValue().Set(ta);                                            \
  }

NEW_TA(NewFloat32Array, v8::Float32Array)
NEW_TA(NewFloat64Array, v8::Float64Array)
NEW_TA(NewInt8Array, v8::Int8Array)
NEW_TA(NewUint8Array, v8::Uint8Array)
NEW_TA(NewUint8ClampedArray, v8::Uint8ClampedArray)
NEW_TA(NewInt16Array, v8::Int16Array)
NEW_TA(NewUint16Array, v8::Uint16Array)
NEW_TA(NewInt32Array, v8::Int32Array)
NEW_TA(NewUint32Array, v8::Uint32Array)

#undef NEW_TA

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, ReadU8);
  NAN_EXPORT(target, ReadI32);
  NAN_EXPORT(target, ReadFloat);
  NAN_EXPORT(target, ReadDouble);

#define TA_EXPORT(x) \
  NAN_EXPORT(target, x); \
  NAN_EXPORT(target, x##FromArrayBuffer);

  TA_EXPORT(NewFloat32Array)
  TA_EXPORT(NewFloat64Array)
  TA_EXPORT(NewInt8Array)
  TA_EXPORT(NewUint8Array)
  TA_EXPORT(NewUint8ClampedArray)
  TA_EXPORT(NewInt16Array)
  TA_EXPORT(NewUint16Array)
  TA_EXPORT(NewInt32Array)
  TA_EXPORT(NewUint32Array)

#undef TA_EXPORT
}

NODE_MODULE(typedarrays, Init)
