/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
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
  NanCallback *callback = new NanCallback(args[0].As<v8::Function>());
  NanAsyncQueueWorker(new ErrorWorker(callback));
  NanReturnUndefined();
}

void Init (v8::Handle<v8::Object> exports) {
  exports->Set(NanNew("a"), NanNew<v8::FunctionTemplate>(Work)->GetFunction());
}

NODE_MODULE(asyncworkererror, Init)
