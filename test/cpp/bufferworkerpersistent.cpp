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

class BufferWorker : public NanAsyncWorker {
 public:
  BufferWorker(
          NanCallback *callback
        , int milliseconds
        , v8::Local<v8::Object> &bufferHandle
      )
    : NanAsyncWorker(callback), milliseconds(milliseconds) {

      NanScope();

      SaveToPersistent("buffer", bufferHandle);
    }

  ~BufferWorker() {}

  void Execute () {
    Sleep(milliseconds);
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

NAN_METHOD(DoSleep) {
  NanScope();
  v8::Local<v8::Object> bufferHandle = args[1].As<v8::Object>();
  NanCallback *callback = new NanCallback(args[2].As<v8::Function>());
  assert(!callback->IsEmpty() && "Callback shoud not be empty");
  NanAsyncQueueWorker(new BufferWorker(
      callback
    , NanUint32Value(args[0]).FromJust()
    , bufferHandle));
  NanReturnUndefined();
}

void Init(v8::Handle<v8::Object> exports) {
  NanSet(exports
    , NanNew<v8::String>("a").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(DoSleep)->GetFunction());
}

NODE_MODULE(bufferworkerpersistent, Init)
