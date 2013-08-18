#include <node.h>
#include "../../nan.h"

NAN_METHOD(ReturnEmptyString) {
  NanScope();
  NanReturnEmptyString();
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnEmptyString)->GetFunction()
  );
}

NODE_MODULE(returnemptystring, Init)

