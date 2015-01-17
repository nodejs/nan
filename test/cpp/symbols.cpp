/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

void Init (v8::Handle<v8::Object> target) {
  target->Set(NanNew<v8::String>("key"), NanNew<v8::String>("a property"));
}

NODE_MODULE(symbols, Init)
