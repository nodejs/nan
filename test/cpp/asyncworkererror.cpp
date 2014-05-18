#include <unistd.h>
#include <nan.h>

class ErrorWorker : public NanAsyncWorker {
 public:
  ErrorWorker(NanCallback *callback) : NanAsyncWorker(callback) {}
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
  exports->Set(NanNew<v8::String>("a"), NanNew<v8::FunctionTemplate>(Work)->GetFunction());
}

NODE_MODULE(asyncworkererror, Init)
