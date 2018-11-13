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

class ProgressWorker : public AsyncProgressWorker {
 public:
  ProgressWorker(
      Callback *callback
    , Callback *progress
    , int milliseconds
    , int iters)
    : AsyncProgressWorker(callback), progress(progress)
    , milliseconds(milliseconds), iters(iters) {}

  ~ProgressWorker() {
    delete progress;
  }

  void Execute (const AsyncProgressWorker::ExecutionProgress& progress) {
    for (int i = 0; i < iters; ++i) {
      progress.Signal();
      Sleep(milliseconds);
    }
  }

  void HandleProgressCallback(const char *data, size_t count) {
    HandleScope scope;

    v8::Local<v8::Value> arg = New<v8::Boolean>(data == NULL && count == 0);
    progress->Call(1, &arg, async_resource);
  }

 private:
  Callback *progress;
  int milliseconds;
  int iters;
};

NAN_METHOD(DoProgress) {
  Callback *progress = new Callback(To<v8::Function>(info[2]).ToLocalChecked());
  Callback *callback = new Callback(To<v8::Function>(info[3]).ToLocalChecked());
  AsyncQueueWorker(new ProgressWorker(
      callback
    , progress
    , To<uint32_t>(info[0]).FromJust()
    , To<uint32_t>(info[1]).FromJust()));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("a").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(DoProgress)).ToLocalChecked());
}

NODE_MODULE(asyncprogressworkersignal, Init)
