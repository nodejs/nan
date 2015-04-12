/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ToBoolean) {
  NanScope();
  NanReturnValue(NanToBoolean(args[0]).ToLocalChecked());
}

NAN_METHOD(ToNumber) {
  NanScope();
  NanReturnValue(NanToNumber(args[0]).ToLocalChecked());
}

NAN_METHOD(ToString) {
  NanScope();
  NanReturnValue(NanToString(args[0]).ToLocalChecked());
}

NAN_METHOD(ToDetailString) {
  NanScope();
  NanReturnValue(NanToDetailString(args[0]).ToLocalChecked());
}

NAN_METHOD(ToObject) {
  NanScope();
  NanReturnValue(NanToObject(args[0]).ToLocalChecked());
}

NAN_METHOD(ToInteger) {
  NanScope();
  NanReturnValue(NanToInteger(args[0]).ToLocalChecked());
}

NAN_METHOD(ToUint32) {
  NanScope();
  NanReturnValue(NanToUint32(args[0]).ToLocalChecked());
}

NAN_METHOD(ToInt32) {
  NanScope();
  NanReturnValue(NanToInt32(args[0]).ToLocalChecked());
}

NAN_METHOD(BooleanValue) {
  NanScope();
  NanReturnValue(NanNew(NanBooleanValue(args[0]).FromJust()));
}

NAN_METHOD(NumberValue) {
  NanScope();
  NanReturnValue(NanNew(NanNumberValue(args[0]).FromJust()));
}

NAN_METHOD(IntegerValue) {
  NanScope();
  NanReturnValue(NanNew<v8::Integer>(static_cast<int32_t>(NanIntegerValue(args[0]).FromJust())));
}

NAN_METHOD(Uint32Value) {
  NanScope();
  NanReturnValue(NanNew<v8::Uint32>(NanUint32Value(args[0]).FromJust()));
}

NAN_METHOD(Int32Value) {
  NanScope();
  NanReturnValue(NanNew<v8::Int32>(NanInt32Value(args[0]).FromJust()));
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
