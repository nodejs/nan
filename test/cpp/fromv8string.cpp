#include <node.h>
#include "../../nan.h"

NAN_METHOD(ReturnString) {
  NanScope();

  NanReturnValue(v8::String::New(NanFromV8String(args[0].As<v8::String>())));
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnString)->GetFunction()
  );
}

NODE_MODULE(fromv8string, Init)
