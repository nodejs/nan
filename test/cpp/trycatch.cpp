/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(TryCatchTest) {
  TryCatch try_catch;
  v8::Local<v8::Value> v =
      RunScript(New<UnboundScript>(
          New("throw 'waaa'").ToLocalChecked()).ToLocalChecked())
              .ToLocalChecked();
  if (v.IsEmpty()) {
    assert(try_catch.HasCaught());
    try_catch.ReThrow();
  }
}

void Init (v8::Handle<v8::Object> target) {
  Set(target
    , New<v8::String>("r").ToLocalChecked()
    , New<v8::FunctionTemplate>(TryCatchTest)->GetFunction()
  );
}

NODE_MODULE(trycatch, Init)
