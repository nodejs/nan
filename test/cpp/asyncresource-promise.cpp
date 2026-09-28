/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

#include "sleep.h"  // NOLINT(build/include_subdir)

using namespace Nan;  // NOLINT(build/namespaces)

class DelayRequest : public AsyncResource {
 public:
  DelayRequest(int milliseconds_, v8::Local<v8::Promise::Resolver> resolver_)
    : AsyncResource("nan:test.DelayPromise"),
      milliseconds(milliseconds_) {
      resolver.Reset(resolver_);
      request.data = this;
    }
  ~DelayRequest() {
    resolver.Reset();
  }

  Persistent<v8::Promise::Resolver> resolver;
  uv_work_t request;
  int milliseconds;
};

void Delay(uv_work_t* req) {
  DelayRequest *delay_request = static_cast<DelayRequest*>(req->data);
  Sleep(delay_request->milliseconds);
}

void AfterDelay(uv_work_t* req, int status) {
  HandleScope scope;

  DelayRequest *delay_request = static_cast<DelayRequest*>(req->data);
  v8::Local<v8::Promise::Resolver> resolver = New(delay_request->resolver);
  resolver->Resolve(Nan::GetCurrentContext(), New<v8::Boolean>(true));

  v8::Local<v8::Object> target = New<v8::Object>();

  // Run the callback in the async context.
  node::CallbackScope callback_scope = delay_request->makeCallbackScope(target);

  delete delay_request;
}

NAN_METHOD(Delay) {
  int delay = To<int>(info[0]).FromJust();
  v8::Local<v8::Promise::Resolver> resolver =
    v8::Promise::Resolver::New(Nan::GetCurrentContext()).ToLocalChecked();

  DelayRequest* delay_request = new DelayRequest(delay, resolver);

  info.GetReturnValue().Set(resolver->GetPromise());

  uv_queue_work(
      GetCurrentEventLoop()
    , &delay_request->request
    , Delay
    , reinterpret_cast<uv_after_work_cb>(AfterDelay));
}

NAN_MODULE_INIT(Init) {
  Set(target, New<v8::String>("delay").ToLocalChecked(),
    GetFunction(New<v8::FunctionTemplate>(Delay)).ToLocalChecked());
}

NODE_MODULE(asyncresource_promise, Init)
