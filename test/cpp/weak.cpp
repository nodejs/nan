/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

template<class T, class P>
struct CallbackInfo {
  CallbackInfo(v8::Handle<T> handle_, P* parameter_)
    : parameter(parameter_) { NanAssignPersistent(handle, handle_);}
  v8::Persistent<T> handle;
  P* parameter;
};


NAN_WEAK_CALLBACK(weakCallback) {
    NanMakeWeak(data.GetParameter()->handle, data.GetParameter(), weakCallback);
}

v8::Handle<v8::String> wrap() {
    NanScope();
    v8::Local<v8::String> lstring = v8::String::New("result");

    int *parameter = new int(3);
    CallbackInfo<v8::String, int> *cbinfo =
      new CallbackInfo<v8::String, int>(lstring, parameter);
    NanMakeWeak(cbinfo->handle, cbinfo, weakCallback);
    return lstring;
}

NAN_METHOD(Hustle) {
  NanScope();
  NanReturnValue(wrap());
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("hustle")
    , v8::FunctionTemplate::New(Hustle)->GetFunction()
  );
}

NODE_MODULE(weak, Init)
