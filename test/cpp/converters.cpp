/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
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
    , New<v8::FunctionTemplate>(ToBoolean)->GetFunction()
  );
  Set(target
    , New<v8::String>("toNumber").ToLocalChecked()
    , New<v8::FunctionTemplate>(ToNumber)->GetFunction()
  );
  Set(target
    , New<v8::String>("toString").ToLocalChecked()
    , New<v8::FunctionTemplate>(ToString)->GetFunction()
  );
  Set(target
    , New<v8::String>("toDetailString").ToLocalChecked()
    , New<v8::FunctionTemplate>(ToDetailString)->GetFunction()
  );
  Set(target
    , New<v8::String>("toObject").ToLocalChecked()
    , New<v8::FunctionTemplate>(ToObject)->GetFunction()
  );
  Set(target
    , New<v8::String>("toInteger").ToLocalChecked()
    , New<v8::FunctionTemplate>(ToInteger)->GetFunction()
  );
  Set(target
    , New<v8::String>("toUint32").ToLocalChecked()
    , New<v8::FunctionTemplate>(ToUint32)->GetFunction()
  );
  Set(target
    , New<v8::String>("toInt32").ToLocalChecked()
    , New<v8::FunctionTemplate>(ToInt32)->GetFunction()
  );
  Set(target
    , New<v8::String>("toArrayIndex").ToLocalChecked()
    , New<v8::FunctionTemplate>(ToArrayIndex)->GetFunction()
  );
  Set(target
    , New<v8::String>("booleanValue").ToLocalChecked()
    , New<v8::FunctionTemplate>(BooleanValue)->GetFunction()
  );
  Set(target
    , New<v8::String>("numberValue").ToLocalChecked()
    , New<v8::FunctionTemplate>(NumberValue)->GetFunction()
  );
  Set(target
    , New<v8::String>("integerValue").ToLocalChecked()
    , New<v8::FunctionTemplate>(IntegerValue)->GetFunction()
  );
  Set(target
    , New<v8::String>("uint32Value").ToLocalChecked()
    , New<v8::FunctionTemplate>(Uint32Value)->GetFunction()
  );
  Set(target
    , New<v8::String>("int32Value").ToLocalChecked()
    , New<v8::FunctionTemplate>(Int32Value)->GetFunction()
  );
}

NODE_MODULE(converters, Init)
