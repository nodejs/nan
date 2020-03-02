/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

static AsyncResource* async_resource;
static Persistent<v8::Function> cb;
void weakCallback(
    const WeakCallbackInfo<int> &data) {  // NOLINT(runtime/references)
  HandleScope scope;
  int *parameter = static_cast<int*>(data.GetInternalField(0));
  v8::Local<v8::Value> val = New(*parameter);
  async_resource->runInAsyncScope(
      GetCurrentContext()->Global(), New(cb), 1, &val);
  delete async_resource;
  delete parameter;
}

v8::Local<v8::String> wrap() {
  EscapableHandleScope scope;
  v8::Local<v8::String> lstring = New("result").ToLocalChecked();
  v8::Local<v8::ObjectTemplate> otpl = New<v8::ObjectTemplate>();
  otpl->SetInternalFieldCount(1);
  v8::Local<v8::Object> obj = NewInstance(otpl).ToLocalChecked();
  SetInternalFieldPointer(obj, 0, new int(42));
  Persistent<v8::Object> persistent(obj);
  persistent.SetWeak(
      static_cast<int*>(0)
    , weakCallback
    , WeakCallbackType::kInternalFields);
  assert(persistent.IsWeak());
  return scope.Escape(lstring);
}

NAN_METHOD(Hustle) {
  async_resource = new AsyncResource("nan:test:weak2");
  cb.Reset(To<v8::Function>(info[0]).ToLocalChecked());
  info.GetReturnValue().Set(wrap());
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("hustle").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(Hustle)).ToLocalChecked()
  );
}

NODE_MODULE(weak2, Init)
