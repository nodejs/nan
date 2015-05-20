/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(TryCatch) {
  NanTryCatch try_catch;
  v8::Local<v8::Value> v =
      NanRunScript(NanNew<NanUnboundScript>(
          NanNew("throw 'waaa'").ToLocalChecked()).ToLocalChecked())
              .ToLocalChecked();
  if (v.IsEmpty()) {
    assert(try_catch.HasCaught());
    try_catch.ReThrow();
  }
}

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("r").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(TryCatch)->GetFunction()
  );
}

NODE_MODULE(trycatch, Init)
