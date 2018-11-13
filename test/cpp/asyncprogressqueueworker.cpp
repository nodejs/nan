/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

class ProgressQueueWorker : public AsyncProgressQueueWorker<char> {
 public:
  ProgressQueueWorker(
      Callback *callback
    , Callback *progress
    , int iters)
    : AsyncProgressQueueWorker(callback), progress(progress)
    , iters(iters) {}

  ~ProgressQueueWorker() {
    delete progress;
  }

  void Execute (const AsyncProgressQueueWorker::ExecutionProgress& progress) {
    for (int i = 0; i < iters; ++i) {
      progress.Send(reinterpret_cast<const char*>(&i), sizeof(int));
    }
  }

  void HandleProgressCallback(const char *data, size_t count) {
    HandleScope scope;

    v8::Local<v8::Value> argv[] = {
        New<v8::Integer>(*reinterpret_cast<int*>(const_cast<char*>(data)))
    };
    progress->Call(1, argv, async_resource);
  }

 private:
  Callback *progress;
  int iters;
};

NAN_METHOD(DoProgress) {
  Callback *progress = new Callback(To<v8::Function>(info[1]).ToLocalChecked());
  Callback *callback = new Callback(To<v8::Function>(info[2]).ToLocalChecked());
  AsyncQueueWorker(new ProgressQueueWorker(
      callback
    , progress
    , To<uint32_t>(info[0]).FromJust()));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("doProgress").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(DoProgress)).ToLocalChecked());
}

NODE_MODULE(asyncprogressqueueworker, Init)
