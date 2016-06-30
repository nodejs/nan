/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef _WIN32
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif
#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

template <class T>
class ProgressWorker : public AsyncProgressWorker<T> {
 public:
  ProgressWorker(
      Callback *callback
    , Callback *progress
    , int milliseconds
    , int iters)
    : AsyncProgressWorker<T>(callback), progress(progress)
    , milliseconds(milliseconds), iters(iters) {}
  ~ProgressWorker() {}

  void Execute (const typename AsyncProgressWorker<T>::ExecutionProgress& progress) {
    for (int i = 0; i < iters; ++i) {
      progress.Signal();
      Sleep(milliseconds);
    }
  }

  void HandleProgressCallback(T *data, size_t size) {
    HandleScope scope;

    v8::Local<v8::Value> arg = New<v8::Boolean>(data == NULL && size == 0);
    progress->Call(1, &arg);
  }

 private:
  Callback *progress;
  int milliseconds;
  int iters;
};

NAN_METHOD(DoProgress) {
  Callback *progress = new Callback(info[2].As<v8::Function>());
  Callback *callback = new Callback(info[3].As<v8::Function>());
  AsyncQueueWorker(new ProgressWorker<char>(
      callback
    , progress
    , To<uint32_t>(info[0]).FromJust()
    , To<uint32_t>(info[1]).FromJust()));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("a").ToLocalChecked()
    , New<v8::FunctionTemplate>(DoProgress)->GetFunction());
}

NODE_MODULE(asyncprogressworkersignal, Init)
