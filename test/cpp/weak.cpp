/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_WEAK_CALLBACK(weakCallback) {
  int *parameter = data.GetParameter();
  NanMakeCallback(NanGetCurrentContext()->Global(), data.GetValue(), 0, NULL);
  if ((*parameter)++ == 0) {
    data.Revive();
  } else {
    delete parameter;
  }
}

v8::Handle<v8::String> wrap(v8::Local<v8::Function> func) {
  v8::Local<v8::String> lstring = NanNew<v8::String>("result").ToLocalChecked();
  int *parameter = new int(0);
  NanMakeWeakPersistent(func, parameter, &weakCallback);
  return lstring;
}

NAN_METHOD(Hustle) {
  NanScope();
  NanReturnValue(wrap(args[0].As<v8::Function>()));
}

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("hustle").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(Hustle)->GetFunction()
  );
}

NODE_MODULE(weak, Init)
