/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#ifndef _WIN32
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif
#include <nan.h>

class SleepWorker : public NanAsyncWorker {
 public:
  SleepWorker(NanCallback *callback, int number)
    : NanAsyncWorker(callback), number(number) {}
  ~SleepWorker() {}

  void Execute () {
    int *number = new int[1];
    number[0] = this->number;
    this->CallIntermediate(number);
  }

  void HandleIntermediate(void *data) {
    NanScope();

    int *number = (int *)data;
    v8::Local<v8::Value> argv[] = {
        NanNull(),
        NanNew<v8::String>("intermediate"),
        NanNew<v8::Integer>(*number)
    };
    callback->Call(3, argv);
    delete[] number;
  }
 private:
  int number;
};

NAN_METHOD(DoSleep) {
  NanScope();
  NanCallback *callback = new NanCallback(args[1].As<v8::Function>());
  NanAsyncQueueWorker(new SleepWorker(callback, args[0]->Uint32Value()));
  NanReturnUndefined();
}

void Init(v8::Handle<v8::Object> exports) {
  exports->Set(
      NanNew<v8::String>("a")
    , NanNew<v8::FunctionTemplate>(DoSleep)->GetFunction());
}

NODE_MODULE(asyncworkerintermediate, Init)
