/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

struct Dummy {
  int value;
};

NAN_METHOD(SetAndGet) {
  NanScope();

  Dummy *d0 = new Dummy;
  Dummy *d1 = NULL;

  v8::Isolate *isolate = v8::Isolate::GetCurrent();

  NanSetIsolateData<Dummy>(isolate, d0);
  d1 = NanGetIsolateData<Dummy>(isolate);

  delete d1;

  NanReturnValue(NanNew<v8::Boolean>(d0 == d1));
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanNew<v8::String>("setAndGet")
    , NanNew<v8::FunctionTemplate>(SetAndGet)->GetFunction()
  );
}

NODE_MODULE(isolatedata, Init)
