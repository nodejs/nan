/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

struct Dummy {
  int value;
};

NAN_METHOD(SetAndGet) {
  Dummy *d0 = new Dummy;
  Dummy *d1 = NULL;

  v8::Isolate *isolate = v8::Isolate::GetCurrent();

  NanSetIsolateData<Dummy>(isolate, d0);
  d1 = NanGetIsolateData<Dummy>(isolate);

  delete d1;

  info.GetReturnValue().Set(NanNew<v8::Boolean>(d0 == d1));
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanNew<v8::String>("setAndGet")
    , NanNew<v8::FunctionTemplate>(SetAndGet)->GetFunction()
  );
}

NODE_MODULE(isolatedata, Init)
