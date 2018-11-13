/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
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
    ret.Set(To<v8::String>(info[0]).ToLocalChecked());
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

NAN_METHOD(ReturnUnsigned) {
  info.GetReturnValue().Set(0x80000000u);
}

NAN_MODULE_INIT(Init) {
  global.Reset(New(true));

  Set(target
    , New<v8::String>("r").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ReturnAValue)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("p").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ReturnPrimitive)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("q").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ReturnGlobal)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("u").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ReturnUnsigned)).ToLocalChecked()
  );
}

NODE_MODULE(returnvalue, Init)
