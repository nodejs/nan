/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ReturnEmptyString) {
  NanReturnEmptyString();
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanNew<v8::String>("r")
    , NanNew<v8::FunctionTemplate>(ReturnEmptyString)->GetFunction()
  );
}

NODE_MODULE(returnemptystring, Init)

