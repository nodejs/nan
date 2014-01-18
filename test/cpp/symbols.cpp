/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <node.h>
#include <nan.h>

void Init (v8::Handle<v8::Object> target) {
  target->Set(NanSymbol("key"), v8::String::New("a property"));
}

NODE_MODULE(symbols, Init)
