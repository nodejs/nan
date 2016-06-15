/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

static Persistent<v8::Function> cb;
static Global<v8::Function> gcb;

void weakCallback(
    const WeakCallbackInfo<int> &data) {  // NOLINT(runtime/references)
  int *parameter = data.GetParameter();
  v8::Local<v8::Value> val = New(*parameter);
  MakeCallback(GetCurrentContext()->Global(), New(cb), 1, &val);
  delete parameter;
}

v8::Local<v8::String> wrap(v8::Local<v8::Function> func) {
  EscapableHandleScope scope;
  v8::Local<v8::String> lstring = New<v8::String>("result").ToLocalChecked();
  int *parameter = new int(42);
  Persistent<v8::Function> persistent(func);
  persistent.SetWeak(parameter, weakCallback, WeakCallbackType::kParameter);
  assert(persistent.IsWeak());
  return scope.Escape(lstring);
}

v8::Local<v8::String> gwrap(v8::Local<v8::Function> func) {
  EscapableHandleScope scope;
  v8::Local<v8::String> lstring = New<v8::String>("result").ToLocalChecked();
  int *parameter = new int(42);
  gcb.Reset(func);
  gcb.SetWeak(parameter, weakCallback, WeakCallbackType::kParameter);
  assert(gcb.IsWeak());
  return scope.Escape(lstring);
}

NAN_METHOD(Hustle) {
  cb.Reset(info[1].As<v8::Function>());
  info.GetReturnValue().Set(wrap(info[0].As<v8::Function>()));
}

NAN_METHOD(HustleGlobal) {
  cb.Reset(info[1].As<v8::Function>());
  info.GetReturnValue().Set(gwrap(info[0].As<v8::Function>()));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("hustle").ToLocalChecked()
    , New<v8::FunctionTemplate>(Hustle)->GetFunction()
  );
  Set(target
    , New<v8::String>("hustleGlobal").ToLocalChecked()
    , New<v8::FunctionTemplate>(HustleGlobal)->GetFunction()
  );
}

NODE_MODULE(weak, Init)
