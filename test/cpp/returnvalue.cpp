/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ReturnValue) {
  NanScope();
  if (args.Length() == 1) {
    NanReturnValue(args[0]);
  } else {
    NanReturnValue(NanNew<v8::String>("default"));
  }
}

NAN_METHOD(ReturnPrimitive) {
  NanScope();
  NanReturnValue(true);
}

NAN_METHOD(ReturnString) {
  NanScope();
  NanReturnValue("yes, it works");
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanNew<v8::String>("r")
    , NanNew<v8::FunctionTemplate>(ReturnValue)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("p")
    , NanNew<v8::FunctionTemplate>(ReturnPrimitive)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("s")
    , NanNew<v8::FunctionTemplate>(ReturnString)->GetFunction()
  );
}

NODE_MODULE(returnvalue, Init)
