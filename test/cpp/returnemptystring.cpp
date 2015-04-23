/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ReturnEmptyString) {
  NanScope();
  NanReturnEmptyString();
}

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("r").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ReturnEmptyString)->GetFunction()
  );
}

NODE_MODULE(returnemptystring, Init)

