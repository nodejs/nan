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
  int *parameter = data.GetParameter();
  v8::Local<v8::Value> val = New(*parameter);
  async_resource->runInAsyncScope(
      GetCurrentContext()->Global(), New(cb), 1, &val);
  delete async_resource;
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

NAN_METHOD(Hustle) {
  async_resource = new AsyncResource("nan:test:weak");
  cb.Reset(To<v8::Function>(info[1]).ToLocalChecked());
  info.GetReturnValue().Set(wrap(To<v8::Function>(info[0]).ToLocalChecked()));
}

inline void WeakExternalCallback(const WeakCallbackInfo<void>&) {}

NAN_METHOD(WeakExternal) {
  void* baton = &baton;  // Actual value doesn't really matter.
  Persistent<v8::External> external(New<v8::External>(baton));
  external.SetWeak(baton, WeakExternalCallback, WeakCallbackType::kParameter);
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("hustle").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(Hustle)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("weakExternal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(WeakExternal)).ToLocalChecked()
  );
}

NODE_MODULE(weak, Init)
