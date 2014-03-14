#include <unistd.h>
#include <node.h>
#include "nan.h"

class ErrorWorker : public NanAsyncWorker {
 public:
  ErrorWorker(NanCallback *callback) : NanAsyncWorker(callback) {}
  ~ErrorWorker() {}

  void Execute () {
    SetErrmsg("Error");
  }
};

NAN_METHOD(Work) {
  NanScope();
  NanCallback *callback = new NanCallback(args[0].As<v8::Function>());
  NanAsyncQueueWorker(new ErrorWorker(callback));
  NanReturnUndefined();
}

void Init (v8::Handle<v8::Object> exports) {
  exports->Set(NanSymbol("a"), v8::FunctionTemplate::New(Work)->GetFunction());
}

NODE_MODULE(asyncworkererror, Init)
