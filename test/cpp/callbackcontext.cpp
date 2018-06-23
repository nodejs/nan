/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

#include "sleep.h"  // NOLINT(build/include)

using namespace Nan;  // NOLINT(build/namespaces)

class DelayRequest : public AsyncResource {
 public:
  DelayRequest(int milliseconds_, v8::Local<v8::Function> callback_)
    : AsyncResource("nan:test.DelayRequest"),
      callback(callback_),
      milliseconds(milliseconds_) {
      request.data = this;
    }
  ~DelayRequest() {}

  Callback callback;
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

  v8::Local<v8::Object> target = New<v8::Object>();

  // Run the callback in the async context.
  delay_request->callback.Call(target, 0, NULL, delay_request);

  delete delay_request;
}

NAN_METHOD(Delay) {
  int delay = To<int>(info[0]).FromJust();
  v8::Local<v8::Function> cb = To<v8::Function>(info[1]).ToLocalChecked();

  DelayRequest* delay_request = new DelayRequest(delay, cb);

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

NODE_MODULE(asyncresource, Init)
