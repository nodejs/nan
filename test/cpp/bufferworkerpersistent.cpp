/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <unistd.h>
#include <node.h>
#include <nan.h>

class BufferWorker : public NanAsyncWorker {
 public:
  BufferWorker(
          NanCallback *callback
        , int milliseconds
        , v8::Local<v8::Object> &bufferHandle
      )
    : NanAsyncWorker(callback), milliseconds(milliseconds) {

      NanScope();

      SavePersistent("buffer", bufferHandle);
    }

  ~BufferWorker() {}

  void Execute () {
    usleep(milliseconds * 1000);
  }

  void HandleOKCallback () {
    NanScope();

    v8::Local<v8::Object> handle = GetFromPersistent("buffer");
    v8::Local<v8::Value> argv[] = { handle };
    callback->Call(1, argv);
  }

 private:
  int milliseconds;
};

NAN_METHOD(Sleep) {
  NanScope();
  v8::Local<v8::Object> bufferHandle = args[1].As<v8::Object>();
  NanCallback *callback = new NanCallback(args[2].As<v8::Function>());
  NanAsyncQueueWorker(new BufferWorker(callback, args[0]->Uint32Value(), bufferHandle));
  NanReturnUndefined();
}

void Init(v8::Handle<v8::Object> exports) {
  exports->Set(
      NanSymbol("a")
    , NanNew<v8::FunctionTemplate>(Sleep)->GetFunction());
}

NODE_MODULE(bufferworkerpersistent, Init)
