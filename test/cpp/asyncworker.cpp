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

class SleepWorker : public AsyncWorker {
 public:
  SleepWorker(Callback *callback, int milliseconds)
    : AsyncWorker(callback, "nan:test.SleepWorker"),
      milliseconds(milliseconds) {}
  ~SleepWorker() {}

  void Execute () {
    Sleep(milliseconds);
  }

 private:
  int milliseconds;
};

NAN_METHOD(DoSleep) {
  Callback *callback = new Callback(To<v8::Function>(info[1]).ToLocalChecked());
  AsyncQueueWorker(
      new SleepWorker(callback, To<uint32_t>(info[0]).FromJust()));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("a").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(DoSleep)).ToLocalChecked());
}

NODE_MODULE(asyncworker, Init)
