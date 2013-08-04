#include <node.h>
#include "../../nan.h"

void Init (v8::Handle<v8::Object> target) {
  target->Set(NanSymbol("key"), v8::String::New("a property"));
}

NODE_MODULE(symbols, Init)
