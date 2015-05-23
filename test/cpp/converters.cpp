/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ToBoolean) {
  info.GetReturnValue().Set(NanTo<v8::Boolean>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToNumber) {
  info.GetReturnValue().Set(NanTo<v8::Number>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToString) {
  info.GetReturnValue().Set(NanTo<v8::String>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToDetailString) {
  info.GetReturnValue().Set(NanToDetailString(info[0]).ToLocalChecked());
}

NAN_METHOD(ToObject) {
  info.GetReturnValue().Set(NanTo<v8::Object>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToInteger) {
  info.GetReturnValue().Set(NanTo<v8::Integer>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToUint32) {
  info.GetReturnValue().Set(NanTo<v8::Uint32>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToInt32) {
  info.GetReturnValue().Set(NanTo<v8::Int32>(info[0]).ToLocalChecked());
}

NAN_METHOD(ToArrayIndex) {
  NanScope();
  info.GetReturnValue().Set(NanToArrayIndex(info[0]).ToLocalChecked());
}

NAN_METHOD(BooleanValue) {
  info.GetReturnValue().Set(NanNew(NanTo<bool>(info[0]).FromJust()));
}

NAN_METHOD(NumberValue) {
  info.GetReturnValue().Set(NanNew(NanTo<double>(info[0]).FromJust()));
}

NAN_METHOD(IntegerValue) {
  info.GetReturnValue().Set(NanNew<v8::Integer>(static_cast<int32_t>(
      NanTo<int64_t>(info[0]).FromJust())));
}

NAN_METHOD(Uint32Value) {
  info.GetReturnValue().Set(NanNew(NanTo<uint32_t>(info[0]).FromJust()));
}

NAN_METHOD(Int32Value) {
  info.GetReturnValue().Set(NanNew(NanTo<int32_t>(info[0]).FromJust()));
}

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("toBoolean").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ToBoolean)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("toNumber").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ToNumber)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("toString").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ToString)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("toDetailString").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ToDetailString)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("toObject").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ToObject)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("toInteger").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ToInteger)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("toUint32").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ToUint32)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("toInt32").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ToInt32)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("toArrayIndex").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ToArrayIndex)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("booleanValue").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(BooleanValue)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("numberValue").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NumberValue)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("integerValue").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(IntegerValue)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("uint32Value").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(Uint32Value)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("int32Value").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(Int32Value)->GetFunction()
  );
}

NODE_MODULE(converters, Init)
