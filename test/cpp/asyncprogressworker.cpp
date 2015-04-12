/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
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
    NanScope();

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
  NanScope();
  NanCallback *progress = new NanCallback(args[2].As<v8::Function>());
  NanCallback *callback = new NanCallback(args[3].As<v8::Function>());
  NanAsyncQueueWorker(new ProgressWorker(
      callback
    , progress
    , NanUint32Value(args[0]).FromJust()
    , NanUint32Value(args[1]).FromJust()));
  NanReturnUndefined();
}

void Init(v8::Handle<v8::Object> exports) {
  NanSet(exports
    , NanNew<v8::String>("a").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(DoProgress)->GetFunction());
}

NODE_MODULE(asyncprogressworker, Init)
