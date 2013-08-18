#include <node.h>
#include "../../nan.h"

NAN_METHOD(ReturnValue) {
  NanScope();
  if (args.Length() == 1) {
    NanReturnValue(args[0]);
  } else {
    NanReturnValue(v8::String::New("default"));
  }
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnValue)->GetFunction()
  );
}

NODE_MODULE(returnvalue, Init)

