/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2017 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

// Custom data type: This serves as an example of how external
// libraries could be hooked in, populate their objects and send them to JS.
struct data_t {
  int index;
  int data;
};

template<typename T>
class ProgressQueueWorker : public AsyncProgressQueueWorker<T> {
 public:
  ProgressQueueWorker(
      Callback *callback
    , Callback *progress
    , int iters)
    : AsyncProgressQueueWorker<T>(callback), progress(progress)
    , iters(iters) {}

  ~ProgressQueueWorker() {
    delete progress;
  }

  void Execute (
    const typename AsyncProgressQueueWorker<T>::ExecutionProgress& progress) {
    data_t data;
    for (int i = 0; i < iters; ++i) {
      data.index = i;
      data.data = i * 2;
      progress.Send(&data, 1);
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
    progress->Call(1, argv);
  }

 private:
  Callback *progress;
  int iters;
};

NAN_METHOD(DoProgress) {
  Callback *progress = new Callback(info[1].As<v8::Function>());
  Callback *callback = new Callback(info[2].As<v8::Function>());
  AsyncQueueWorker(new ProgressQueueWorker<data_t>(
      callback
    , progress
    , To<uint32_t>(info[0]).FromJust()));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("doProgress").ToLocalChecked()
    , New<v8::FunctionTemplate>(DoProgress)->GetFunction());
}

NODE_MODULE(asyncprogressqueueworkerstream, Init)
