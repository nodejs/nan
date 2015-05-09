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

class BufferWorker : public NanAsyncWorker {
 public:
  BufferWorker(
          NanCallback *callback
        , int milliseconds
        , v8::Local<v8::Object> &bufferHandle
      )
    : NanAsyncWorker(callback), milliseconds(milliseconds) {

      NanScope();

     /* test them all */
      SaveToPersistent("buffer", bufferHandle);
      SaveToPersistent(NanNew("puffer"), bufferHandle);
      SaveToPersistent(0u, bufferHandle);
    }

  ~BufferWorker() {}

  void Execute () {
    Sleep(milliseconds);
  }

  void HandleOKCallback () {
    NanScope();

    v8::Local<v8::Value> handle = GetFromPersistent("buffer");
    callback->Call(1, &handle);

    handle = GetFromPersistent(NanNew("puffer"));
    callback->Call(1, &handle);

    handle = GetFromPersistent(0u);
    callback->Call(1, &handle);
  }

 private:
  int milliseconds;
};

NAN_METHOD(DoSleep) {
  NanScope();
  v8::Local<v8::Object> bufferHandle = args[1].As<v8::Object>();
  NanCallback *callback = new NanCallback(args[2].As<v8::Function>());
  assert(!callback->IsEmpty() && "Callback shoud not be empty");
  NanAsyncQueueWorker(
      new BufferWorker(callback, args[0]->Uint32Value(), bufferHandle));
  NanReturnUndefined();
}

void Init(v8::Handle<v8::Object> exports) {
  exports->Set(
      NanNew<v8::String>("a")
    , NanNew<v8::FunctionTemplate>(DoSleep)->GetFunction());
}

NODE_MODULE(bufferworkerpersistent, Init)
