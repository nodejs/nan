/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(ToBoolean) {
  info.GetReturnValue().Set(To<v8::Boolean>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToNumber) {
  info.GetReturnValue().Set(To<v8::Number>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToString) {
  info.GetReturnValue().Set(To<v8::String>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToDetailString) {
  info.GetReturnValue().Set(ToDetailString(info[0]).ToLocalChecked());
}

NAN_METHOD(ToFunction) {
  info.GetReturnValue().Set(To<v8::Function>(info[0]).FromMaybe(info[1]));
}

NAN_METHOD(ToObject) {
  info.GetReturnValue().Set(To<v8::Object>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToInteger) {
  info.GetReturnValue().Set(To<v8::Integer>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToUint32) {
  info.GetReturnValue().Set(To<v8::Uint32>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToInt32) {
  info.GetReturnValue().Set(To<v8::Int32>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToArrayIndex) {
  HandleScope scope;
  info.GetReturnValue().Set(ToArrayIndex(info[0]).ToLocalChecked());
}

NAN_METHOD(BooleanValue) {
  info.GetReturnValue().Set(New(To<bool>(info[0]).FromJust()));
}

NAN_METHOD(NumberValue) {
  info.GetReturnValue().Set(New(To<double>(info[0]).FromJust()));
}

NAN_METHOD(IntegerValue) {
  info.GetReturnValue().Set(New<v8::Integer>(static_cast<int32_t>(
      To<int64_t>(info[0]).FromJust())));
}

NAN_METHOD(Uint32Value) {
  info.GetReturnValue().Set(New(To<uint32_t>(info[0]).FromJust()));
}

NAN_METHOD(Int32Value) {
  info.GetReturnValue().Set(New(To<int32_t>(info[0]).FromJust()));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("toBoolean").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToBoolean)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toNumber").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToNumber)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toString").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToString)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toDetailString").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToDetailString)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toFunction").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToFunction)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toObject").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToObject)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toInteger").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToInteger)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toUint32").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToUint32)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toInt32").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToInt32)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toArrayIndex").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToArrayIndex)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("booleanValue").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(BooleanValue)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("numberValue").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NumberValue)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("integerValue").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(IntegerValue)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("uint32Value").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(Uint32Value)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("int32Value").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(Int32Value)).ToLocalChecked()
  );
}

NODE_MODULE(converters, Init)
