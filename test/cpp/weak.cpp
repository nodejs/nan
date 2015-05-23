/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

static NanPersistent<v8::Function> cb;

void weakCallback(
    const NanWeakCallbackInfo<int> &data) {  // NOLINT(runtime/references)
  int *parameter = data.GetParameter();
  v8::Local<v8::Value> val = NanNew(*parameter);
  NanMakeCallback(NanGetCurrentContext()->Global(), NanNew(cb), 1, &val);
  delete parameter;
}

v8::Handle<v8::String> wrap(v8::Local<v8::Function> func) {
  NanEscapableScope scope;
  v8::Local<v8::String> lstring = NanNew<v8::String>("result").ToLocalChecked();
  int *parameter = new int(42);
  NanPersistent<v8::Function> persistent(func);
  persistent.SetWeak(parameter, weakCallback, NanWeakCallbackType::kParameter);
  assert(persistent.IsWeak());
  return scope.Escape(lstring);
}

NAN_METHOD(Hustle) {
  cb.Reset(info[1].As<v8::Function>());
  info.GetReturnValue().Set(wrap(info[0].As<v8::Function>()));
}

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("hustle").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(Hustle)->GetFunction()
  );
}

NODE_MODULE(weak, Init)
