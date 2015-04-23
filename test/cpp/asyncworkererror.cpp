/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

class ErrorWorker : public NanAsyncWorker {
 public:
  explicit ErrorWorker(NanCallback *callback) : NanAsyncWorker(callback) {}
  ~ErrorWorker() {}

  void Execute () {
    SetErrorMessage("Error");
  }
};

NAN_METHOD(Work) {
  NanScope();
  NanCallback *callback = new NanCallback(args[0].As<v8::Function>());
  NanAsyncQueueWorker(new ErrorWorker(callback));
  NanReturnUndefined();
}

void Init (v8::Handle<v8::Object> exports) {
  NanSet(exports
    , NanNew("a").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(Work)->GetFunction());
}

NODE_MODULE(asyncworkererror, Init)
