/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NanGlobal<v8::Boolean> global;

NAN_METHOD(ReturnValue) {
  const NanFunctionCallbackInfo<v8::Value> cbinfo = info;
  NanReturnValue<v8::Value> ret = cbinfo.GetReturnValue();
  if (cbinfo.Length() == 1) {
    ret.Set(info[0].As<v8::String>());
  } else {
    ret.Set(NanNew("default").ToLocalChecked());
  }
}

NAN_METHOD(ReturnPrimitive) {
  info.GetReturnValue().Set(true);
}

NAN_METHOD(ReturnGlobal) {
  info.GetReturnValue().Set(global);
}

void Init (v8::Handle<v8::Object> target) {
  global.Reset(NanNew(true));

  NanSet(target
    , NanNew<v8::String>("r").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ReturnValue)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("p").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ReturnPrimitive)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("q").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ReturnGlobal)->GetFunction()
  );
}

NODE_MODULE(returnvalue, Init)
