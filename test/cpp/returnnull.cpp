#include <node.h>
#include "../../nan.h"

NAN_METHOD(ReturnNull) {
  NanScope();
  NanReturnNull();
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnNull)->GetFunction()
  );
}

NODE_MODULE(returnnull, Init)

