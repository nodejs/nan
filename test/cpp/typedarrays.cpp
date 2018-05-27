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

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, ReadU8);
  NAN_EXPORT(target, ReadI32);
  NAN_EXPORT(target, ReadFloat);
  NAN_EXPORT(target, ReadDouble);
}

NODE_MODULE(typedarrays, Init)
