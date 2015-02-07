/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>


NAN_METHOD(GlobalContext) {
  NanScope();

  NanCallback(args[0].As<v8::Function>()).Call(0, NULL);
  NanReturnUndefined();
}

NAN_METHOD(SpecificContext) {
  NanScope();

  NanCallback cb(args[0].As<v8::Function>());
  cb.Call(NanGetCurrentContext()->Global(), 0, NULL);
  NanReturnUndefined();
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanNew<v8::String>("globalContext")
    , NanNew<v8::FunctionTemplate>(GlobalContext)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("specificContext")
    , NanNew<v8::FunctionTemplate>(SpecificContext)->GetFunction()
  );
}

NODE_MODULE(nancallback, Init)
