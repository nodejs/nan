/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NanGlobal<v8::Boolean> global;

NAN_METHOD(ReturnValue) {
  if (args.Length() == 1) {
    NanReturnValue(args[0]);
  } else {
    NanReturnValue(NanNew<v8::String>("default"));
  }
}

NAN_METHOD(ReturnPrimitive) {
  NanReturnValue(true);
}

NAN_METHOD(ReturnString) {
  NanReturnValue("yes, it works");
}

NAN_METHOD(ReturnGlobal) {
  NanReturnValue(global);
}

void Init (v8::Handle<v8::Object> target) {
  global.Reset(NanNew(true));

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
  target->Set(
      NanNew<v8::String>("q")
    , NanNew<v8::FunctionTemplate>(ReturnGlobal)->GetFunction()
  );
}

NODE_MODULE(returnvalue, Init)
