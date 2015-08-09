/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

static Global<v8::Boolean> global;

NAN_METHOD(ReturnAValue) {
  const FunctionCallbackInfo<v8::Value> &cbinfo = info;
  ReturnValue<v8::Value> ret = cbinfo.GetReturnValue();
  if (cbinfo.Length() == 1) {
    ret.Set(info[0].As<v8::String>());
  } else {
    ret.Set(New("default").ToLocalChecked());
  }
}

NAN_METHOD(ReturnPrimitive) {
  info.GetReturnValue().Set(true);
}

NAN_METHOD(ReturnGlobal) {
  info.GetReturnValue().Set(global);
}

NAN_MODULE_INIT(Init) {
  global.Reset(New(true));

  Set(target
    , New<v8::String>("r").ToLocalChecked()
    , New<v8::FunctionTemplate>(ReturnAValue)->GetFunction()
  );
  Set(target
    , New<v8::String>("p").ToLocalChecked()
    , New<v8::FunctionTemplate>(ReturnPrimitive)->GetFunction()
  );
  Set(target
    , New<v8::String>("q").ToLocalChecked()
    , New<v8::FunctionTemplate>(ReturnGlobal)->GetFunction()
  );
}

NODE_MODULE(returnvalue, Init)
