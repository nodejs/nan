/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

#include <stdint.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(ReadU8) {
  Nan::TypedArrayContents<uint8_t> data(info[0]);

  v8::Local<v8::Array> result = Nan::New<v8::Array>(data.length());
  for (size_t i=0; i<data.length(); ++i) {
    Nan::Set(result, i, Nan::New<v8::Number>((*data)[i]));
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(ReadI32) {
  Nan::TypedArrayContents<int32_t> data(info[0]);

  v8::Local<v8::Array> result = Nan::New<v8::Array>(data.length());
  for (size_t i=0; i<data.length(); ++i) {
    Nan::Set(result, i, Nan::New<v8::Number>((*data)[i]));
  }

  info.GetReturnValue().Set(result);
}


NAN_METHOD(ReadFloat) {
  Nan::TypedArrayContents<float> data(info[0]);

  v8::Local<v8::Array> result = Nan::New<v8::Array>(data.length());
  for (size_t i=0; i<data.length(); ++i) {
    Nan::Set(result, i, Nan::New<v8::Number>((*data)[i]));
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(ReadDouble) {
  Nan::TypedArrayContents<double> data(info[0]);

  v8::Local<v8::Array> result = Nan::New<v8::Array>(data.length());
  for (size_t i=0; i<data.length(); ++i) {
    Nan::Set(result, i, Nan::New<v8::Number>((*data)[i]));
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
