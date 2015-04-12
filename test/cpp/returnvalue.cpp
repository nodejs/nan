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
    NanReturnValue(NanNew<v8::String>("default").ToLocalChecked());
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
  NanSet(target
    , NanNew<v8::String>("r").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ReturnValue)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("p").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ReturnPrimitive)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("s").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ReturnString)->GetFunction()
  );
}

NODE_MODULE(returnvalue, Init)
