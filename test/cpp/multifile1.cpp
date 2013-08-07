#include <node.h>
#include "../../nan.h"
#include "multifile2.h"

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnString)->GetFunction()
  );
}

NODE_MODULE(multifile, Init)
