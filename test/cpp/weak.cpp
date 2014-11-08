/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

void weakCallback(
NanWeakCallbackData<v8::Function, int> & data) {  // NOLINT(runtime/references)
  int *parameter = data.GetParameter();
  NanMakeCallback(NanGetCurrentContext()->Global(), data.GetValue(), 0, NULL);
  if ((*parameter)++ == 0) {
    data.Revive();
  } else {
    delete parameter;
  }
}

v8::Handle<v8::String> wrap(v8::Local<v8::Function> func) {
  NanEscapableScope();
  v8::Local<v8::String> lstring = NanNew<v8::String>("result");
  int *parameter = new int(0);
  NanMakeWeakPersistent(func, parameter, weakCallback);
  return NanEscapeScope(lstring);
}

NAN_METHOD(Hustle) {
  NanScope();
  NanReturnValue(wrap(args[0].As<v8::Function>()));
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanNew<v8::String>("hustle")
    , NanNew<v8::FunctionTemplate>(Hustle)->GetFunction()
  );
}

NODE_MODULE(weak, Init)
