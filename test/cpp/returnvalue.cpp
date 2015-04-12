/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

v8::Persistent<v8::Boolean> persistent;

NAN_METHOD(ReturnValue) {
  if (args.Length() == 1) {
    NanReturnValue(args[0]);
  } else {
    NanReturnValue(NanNew<v8::String>("default").ToLocalChecked());
  }
}

NAN_METHOD(ReturnPrimitive) {
  NanReturnValue(true);
}

NAN_METHOD(ReturnString) {
  NanReturnValue("yes, it works");
}

NAN_METHOD(ReturnPersistent) {
  NanReturnValue(persistent);
}

void Init (v8::Handle<v8::Object> target) {
  NanAssignPersistent(persistent, NanNew(true));
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
  NanSet(target
    , NanNew<v8::String>("q").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ReturnPersistent)->GetFunction()
  );
}

NODE_MODULE(returnvalue, Init)
