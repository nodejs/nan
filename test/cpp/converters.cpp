/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ToBoolean) {
  NanScope();
  NanReturnValue(NanTo<v8::Boolean>(args[0]).ToLocalChecked());
}

NAN_METHOD(ToNumber) {
  NanScope();
  NanReturnValue(NanTo<v8::Number>(args[0]).ToLocalChecked());
}

NAN_METHOD(ToString) {
  NanScope();
  NanReturnValue(NanTo<v8::String>(args[0]).ToLocalChecked());
}

NAN_METHOD(ToDetailString) {
  NanScope();
  NanReturnValue(NanToDetailString(args[0]).ToLocalChecked());
}

NAN_METHOD(ToObject) {
  NanScope();
  NanReturnValue(NanTo<v8::Object>(args[0]).ToLocalChecked());
}

NAN_METHOD(ToInteger) {
  NanScope();
  NanReturnValue(NanTo<v8::Integer>(args[0]).ToLocalChecked());
}

NAN_METHOD(ToUint32) {
  NanScope();
  NanReturnValue(NanTo<v8::Uint32>(args[0]).ToLocalChecked());
}

NAN_METHOD(ToInt32) {
  NanScope();
  NanReturnValue(NanTo<v8::Int32>(args[0]).ToLocalChecked());
}

NAN_METHOD(ToArrayIndex) {
  NanScope();
  NanReturnValue(NanToArrayIndex(args[0]).ToLocalChecked());
}

NAN_METHOD(BooleanValue) {
  NanScope();
  NanReturnValue(NanNew(NanTo<bool>(args[0]).FromJust()));
}

NAN_METHOD(NumberValue) {
  NanScope();
  NanReturnValue(NanNew(NanTo<double>(args[0]).FromJust()));
}

NAN_METHOD(IntegerValue) {
  NanScope();
  NanReturnValue(NanNew<v8::Integer>(static_cast<int32_t>(
      NanTo<int64_t>(args[0]).FromJust())));
}

NAN_METHOD(Uint32Value) {
  NanScope();
  NanReturnValue(NanNew<v8::Uint32>(NanTo<uint32_t>(args[0]).FromJust()));
}

NAN_METHOD(Int32Value) {
  NanScope();
  NanReturnValue(NanNew<v8::Int32>(NanTo<int32_t>(args[0]).FromJust()));
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
