#include <node.h>
#include "../../nan.h"

NAN_METHOD(ReturnString) {
  NanScope();
  node::encoding enc = node::UTF8;
  size_t bc;

  if (args[1]->IsUint32()) {
    enc = (node::encoding) args[1]->Uint32Value();
  }

  char *s = NanFromV8String(args[0].As<v8::Object>(), enc, &bc);
  NanReturnValue(v8::String::New(s));
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnString)->GetFunction()
  );
}

NODE_MODULE(fromv8string, Init)
