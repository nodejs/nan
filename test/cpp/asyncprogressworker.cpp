/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef _WIN32
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif
#include <nan.h>

class ProgressWorker : public NanAsyncProgressWorker {
 public:
  ProgressWorker(
      NanCallback *callback
    , NanCallback *progress
    , int milliseconds
    , int iters)
    : NanAsyncProgressWorker(callback), progress(progress)
    , milliseconds(milliseconds), iters(iters) {}
  ~ProgressWorker() {}

  void Execute (const NanAsyncProgressWorker::ExecutionProgress& progress) {
    for (int i = 0; i < iters; ++i) {
      progress.Send(reinterpret_cast<const char*>(&i), sizeof(int));
      Sleep(milliseconds);
    }
  }

  void HandleProgressCallback(const char *data, size_t size) {
    NanScope scope;

    v8::Local<v8::Value> argv[] = {
        NanNew<v8::Integer>(*reinterpret_cast<int*>(const_cast<char*>(data)))
    };
    progress->Call(1, argv);
  }

 private:
  NanCallback *progress;
  int milliseconds;
  int iters;
};

NAN_METHOD(DoProgress) {
  NanCallback *progress = new NanCallback(info[2].As<v8::Function>());
  NanCallback *callback = new NanCallback(info[3].As<v8::Function>());
  NanAsyncQueueWorker(new ProgressWorker(
      callback
    , progress
    , info[0]->Uint32Value()
    , info[1]->Uint32Value()));
  info.GetReturnValue().SetUndefined();
}

void Init(v8::Handle<v8::Object> exports) {
  exports->Set(
      NanNew<v8::String>("a")
    , NanNew<v8::FunctionTemplate>(DoProgress)->GetFunction());
}

NODE_MODULE(asyncprogressworker, Init)
