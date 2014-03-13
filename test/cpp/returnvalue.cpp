/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <node.h>
#include <nan.h>

NAN_METHOD(ReturnValue) {
  NanScope();
  if (args.Length() == 1) {
    NanReturnValue(args[0]);
  } else {
    NanReturnValue(NanNew<v8::String>("default"));
  }
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , NanNew<v8::FunctionTemplate>(ReturnValue)->GetFunction()
  );
}

NODE_MODULE(returnvalue, Init)
