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

// Custom data type: This serves as an example of how external
// libraries could be hooked in, populate their objects and send them to JS.
struct data_t {
  int index;
  int data;
};

// Unlike test/cpp/ayncprogressworker.cpp this test is explicitly templated.
template<typename T>
class ProgressWorker : public AsyncProgressWorkerBase<T> {
 public:
  ProgressWorker(
      Callback *callback
    , Callback *progress
    , int milliseconds
    , int iters)
    : AsyncProgressWorkerBase<T>(callback), progress(progress)
    , milliseconds(milliseconds), iters(iters) {}

  ~ProgressWorker() {
    delete progress;
  }

  void Execute (
    const typename AsyncProgressWorkerBase<T>::ExecutionProgress& progress) {
    data_t data;
    for (int i = 0; i < iters; ++i) {
      data.index = i;
      data.data = i * 2;
      progress.Send(&data, 1);
      Sleep(milliseconds);
    }
  }

  void HandleProgressCallback(const T *data, size_t count) {
    HandleScope scope;
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    Nan::Set(
      obj,
      Nan::New("index").ToLocalChecked(),
      New<v8::Integer>(data->index));
    Nan::Set(
      obj,
      Nan::New("data").ToLocalChecked(),
      New<v8::Integer>(data->data));

    v8::Local<v8::Value> argv[] = { obj };
    progress->Call(1, argv, this->async_resource);
  }

 private:
  Callback *progress;
  int milliseconds;
  int iters;
};

NAN_METHOD(DoProgress) {
  Callback *progress = new Callback(To<v8::Function>(info[2]).ToLocalChecked());
  Callback *callback = new Callback(To<v8::Function>(info[3]).ToLocalChecked());
  AsyncQueueWorker(new ProgressWorker<data_t>(
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

NODE_MODULE(asyncprogressworkerstream, Init)
