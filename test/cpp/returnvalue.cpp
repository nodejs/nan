/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

v8::Persistent<v8::Boolean> persistent;

NAN_METHOD(ReturnValue) {
  const NanFunctionCallbackInfo<v8::Value> cbinfo = info;
  NanReturnValue<v8::Value> ret = cbinfo.GetReturnValue();
  if (cbinfo.Length() == 1) {
    ret.Set(info[0].As<v8::String>());
  } else {
    ret.Set(NanNew<v8::String>("default"));
  }
}

NAN_METHOD(ReturnPrimitive) {
  info.GetReturnValue().Set(true);
}

NAN_METHOD(ReturnPersistent) {
  info.GetReturnValue().Set(persistent);
}

void Init (v8::Handle<v8::Object> target) {
  NanAssignPersistent(persistent, NanNew(true));

  target->Set(
      NanNew<v8::String>("r")
    , NanNew<v8::FunctionTemplate>(ReturnValue)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("p")
    , NanNew<v8::FunctionTemplate>(ReturnPrimitive)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("q")
    , NanNew<v8::FunctionTemplate>(ReturnPersistent)->GetFunction()
  );
}

NODE_MODULE(returnvalue, Init)
