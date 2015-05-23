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

class SleepWorker : public NanAsyncWorker {
 public:
  SleepWorker(NanCallback *callback, int milliseconds)
    : NanAsyncWorker(callback), milliseconds(milliseconds) {}
  ~SleepWorker() {}

  void Execute () {
    Sleep(milliseconds);
  }

 private:
  int milliseconds;
};

NAN_METHOD(DoSleep) {
  NanCallback *callback = new NanCallback(info[1].As<v8::Function>());
  NanAsyncQueueWorker(
      new SleepWorker(callback, NanTo<uint32_t>(info[0]).FromJust()));
}

void Init(v8::Handle<v8::Object> exports) {
  NanSet(exports
    , NanNew<v8::String>("a").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(DoSleep)->GetFunction());
}

NODE_MODULE(asyncworker, Init)
