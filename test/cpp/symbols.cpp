/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

void Init (v8::Handle<v8::Object> target) {
  target->Set(NanNew<v8::String>("key"), NanNew<v8::String>("a property"));
}

NODE_MODULE(symbols, Init)
