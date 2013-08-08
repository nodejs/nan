#include <node.h>
#include "../../nan.h"

NAN_METHOD(ReturnString) {
  NanScope();
  Nan::Encoding enc = Nan::UTF8;
  size_t bc;

  if (args[1]->IsUint32()) {
    enc = (Nan::Encoding) args[1]->Uint32Value();
  }

  char *s = NanFromV8String(args[0].As<v8::Object>(), enc, &bc);
  if (enc == Nan::UCS2) {
    NanReturnValue(v8::String::New(reinterpret_cast<uint16_t *>(s), bc / 2));
  } else {
    NanReturnValue(v8::String::New(s, bc));
  }
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnString)->GetFunction()
  );
}

NODE_MODULE(fromv8string, Init)
