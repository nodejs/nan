/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

class ErrorWorker : public AsyncWorker {
 public:
  explicit ErrorWorker(Callback *callback) : AsyncWorker(callback) {}
  ~ErrorWorker() {}

  void Execute () {
    SetErrorMessage("Error");
  }
};

NAN_METHOD(Work) {
  Callback *callback = new Callback(info[0].As<v8::Function>());
  AsyncQueueWorker(new ErrorWorker(callback));
  info.GetReturnValue().SetUndefined();
}

void Init(v8::Local<v8::Object> target) {
  Set(target
    , New("a").ToLocalChecked()
    , New<v8::FunctionTemplate>(Work)->GetFunction());
}

NAN_MODULE(asyncworkererror, Init)
