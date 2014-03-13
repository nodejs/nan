/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

NAN_WEAK_CALLBACK(weakCallback) {
    int *parameter = data.GetParameter();

# if NODE_MODULE_VERSION > 0x000B
      node::MakeCallback(nan_isolate->GetCurrentContext()->Global(),
        data.GetValue(), 0, NULL);
#else
    #if NODE_VERSION_AT_LEAST(0, 8, 0)
      node::MakeCallback(nan_isolate, v8::Context::GetCurrent()->Global(),
        data.GetValue(), 0, NULL);
    #else
      v8::TryCatch try_catch;
      data.GetValue()->Call(v8::Null(), 0, NULL);

      if (try_catch.HasCaught()) {
        FatalException(try_catch);
      }
    #endif
#endif

    if ((*parameter)++ == 0) {
      data.Revive();
    } else {
      data.Dispose();
    }
}

v8::Handle<v8::String> wrap(v8::Local<v8::Function> func) {
    NanScope();
    v8::Local<v8::String> lstring = NanNew<v8::String>("result");
    int *parameter = new int(0);
    NanMakeWeakPersistent(func, parameter, weakCallback);
    return lstring;
}

NAN_METHOD(Hustle) {
  NanScope();
  NanReturnValue(wrap(args[0].As<v8::Function>()));
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("hustle")
    , NanNew<v8::FunctionTemplate>(Hustle)->GetFunction()
  );
}

NODE_MODULE(weak, Init)
