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

using namespace Nan;  // NOLINT(build/namespaces)

class BufferWorker : public NanAsyncWorker {
 public:
  BufferWorker(
          NanCallback *callback
        , int milliseconds
        , v8::Local<v8::Object> &bufferHandle
      )
    : NanAsyncWorker(callback), milliseconds(milliseconds) {
      SaveToPersistent("buffer", bufferHandle);
      SaveToPersistent(NanNew("puffer").ToLocalChecked(), bufferHandle);
      SaveToPersistent(0u, bufferHandle);
    }

  ~BufferWorker() {}

  void Execute () {
    Sleep(milliseconds);
  }

  void HandleOKCallback () {
    NanScope scope;

    v8::Local<v8::Value> handle = GetFromPersistent("buffer");
    callback->Call(1, &handle);

    handle = GetFromPersistent(NanNew("puffer").ToLocalChecked());
    callback->Call(1, &handle);

    handle = GetFromPersistent(0u);
    callback->Call(1, &handle);
  }

 private:
  int milliseconds;
};

NAN_METHOD(DoSleep) {
  v8::Local<v8::Object> bufferHandle = info[1].As<v8::Object>();
  NanCallback *callback = new NanCallback(info[2].As<v8::Function>());
  assert(!callback->IsEmpty() && "Callback shoud not be empty");
  NanAsyncQueueWorker(new BufferWorker(
      callback
    , NanTo<uint32_t>(info[0]).FromJust()
    , bufferHandle));
}

void Init(v8::Handle<v8::Object> exports) {
  NanSet(exports
    , NanNew<v8::String>("a").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(DoSleep)->GetFunction());
}

NODE_MODULE(bufferworkerpersistent, Init)
