/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
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

NAN_MODULE_INIT(Init) {
  Set(target
    , New("a").ToLocalChecked()
    , New<v8::FunctionTemplate>(Work)->GetFunction());
}

NODE_MODULE(asyncworkererror, Init)
