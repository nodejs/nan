/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

struct Dummy {
  int value;
};

NAN_METHOD(SetAndGet) {
  Dummy *d0 = new Dummy;
  Dummy *d1 = NULL;

  v8::Isolate *isolate = v8::Isolate::GetCurrent();

  SetIsolateData<Dummy>(isolate, d0);
  d1 = GetIsolateData<Dummy>(isolate);

  delete d1;

  info.GetReturnValue().Set(New<v8::Boolean>(d0 == d1));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("setAndGet").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(SetAndGet)).ToLocalChecked()
  );
}

NODE_MODULE(isolatedata, Init)
