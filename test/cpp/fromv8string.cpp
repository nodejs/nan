#include <node.h>
#include "../../nan.h"

NAN_METHOD(ReturnString) {
  NanScope();
  Nan::Encoding enc = Nan::UTF8;
  size_t bc;
  unsigned int flags = v8::String::HINT_MANY_WRITES_EXPECTED | v8::String::NO_NULL_TERMINATION;

  if (args[1]->IsUint32()) {
    enc = (Nan::Encoding) args[1]->Uint32Value();
  }

  if (args[2]->IsUint32()) {
    flags = args[2]->Uint32Value();
  }

  char *s = NanFromV8String(args[0].As<v8::Object>(), enc, &bc, NULL, 0, flags);
  if (enc == Nan::UCS2) {
    NanReturnValue(v8::String::New(reinterpret_cast<uint16_t *>(s), flags & v8::String::NO_NULL_TERMINATION ? bc / 2 : - 1));
  } else {
    NanReturnValue(v8::String::New(s, flags & v8::String::NO_NULL_TERMINATION ? bc : -1));
  }
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnString)->GetFunction()
  );
}

NODE_MODULE(fromv8string, Init)
