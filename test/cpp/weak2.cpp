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
  int *parameter = static_cast<int*>(data.GetInternalField(0));
  v8::Local<v8::Value> val = NanNew(*parameter);
  NanMakeCallback(NanGetCurrentContext()->Global(), NanNew(cb), 1, &val);
  delete parameter;
}

v8::Handle<v8::String> wrap() {
  NanEscapableScope scope;
  v8::Local<v8::String> lstring = NanNew("result").ToLocalChecked();
  v8::Local<v8::ObjectTemplate> otpl = NanNew<v8::ObjectTemplate>();
  otpl->SetInternalFieldCount(1);
  v8::Local<v8::Object> obj = otpl->NewInstance();
  NanSetInternalFieldPointer(obj, 0, new int(42));
  NanPersistent<v8::Object> persistent(obj);
  persistent.SetWeak(
      static_cast<int*>(0)
    , weakCallback
    , NanWeakCallbackType::kInternalFields);
  assert(persistent.IsWeak());
  return scope.Escape(lstring);
}

NAN_METHOD(Hustle) {
  cb.Reset(info[0].As<v8::Function>());
  info.GetReturnValue().Set(wrap());
}

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("hustle").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(Hustle)->GetFunction()
  );
}

NODE_MODULE(weak2, Init)
