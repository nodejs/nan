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

class BufferWorker : public AsyncWorker {
 public:
  BufferWorker(
          Callback *callback
        , int milliseconds
        , v8::Local<v8::Object> &bufferHandle
      )
    : AsyncWorker(callback), milliseconds(milliseconds) {
      SaveToPersistent("buffer", bufferHandle);
      SaveToPersistent(New("puffer").ToLocalChecked(), bufferHandle);
      SaveToPersistent(0u, bufferHandle);
    }

  ~BufferWorker() {}

  void Execute () {
    Sleep(milliseconds);
  }

  void HandleOKCallback () {
    HandleScope scope;

    v8::Local<v8::Value> handle = GetFromPersistent("buffer");
    callback->Call(1, &handle, async_resource);

    handle = GetFromPersistent(New("puffer").ToLocalChecked());
    callback->Call(1, &handle, async_resource);

    handle = GetFromPersistent(0u);
    callback->Call(1, &handle, async_resource);
  }

 private:
  int milliseconds;
};

NAN_METHOD(DoSleep) {
  v8::Local<v8::Object> bufferHandle = To<v8::Object>(info[1]).ToLocalChecked();
  Callback *callback = new Callback(To<v8::Function>(info[2]).ToLocalChecked());
  assert(!callback->IsEmpty() && "Callback shoud not be empty");
  AsyncQueueWorker(new BufferWorker(
      callback
    , To<uint32_t>(info[0]).FromJust()
    , bufferHandle));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("a").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(DoSleep)).ToLocalChecked());
}

NODE_MODULE(bufferworkerpersistent, Init)
