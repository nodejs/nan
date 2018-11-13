/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(TryCatchTest) {
  TryCatch try_catch;
  v8::Local<v8::String> s = New("throw 'waaa'").ToLocalChecked();
  v8::Local<UnboundScript> script = New<UnboundScript>(s).ToLocalChecked();
  MaybeLocal<v8::Value> result = RunScript(script);
  if (result.IsEmpty()) {
    assert(try_catch.HasCaught());
    try_catch.ReThrow();
  }
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("r").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(TryCatchTest)).ToLocalChecked()
  );
}

NODE_MODULE(trycatch, Init)
