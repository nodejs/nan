#include <node.h>
#include "nan.h"

NAN_METHOD(ReturnString) {
  NanScope();
  size_t bc;

  char *s = NanCString(args[0].As<v8::Object>(), &bc);
  v8::Local<v8::String> str = v8::String::New(s);
  delete[] s;
  NanReturnValue(str);
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnString)->GetFunction()
  );
}

NODE_MODULE(cstring, Init)
