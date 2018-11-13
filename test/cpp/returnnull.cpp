/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ReturnNull) {
  info.GetReturnValue().SetNull();
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("r").ToLocalChecked()
    , Nan::GetFunction(Nan::New<v8::FunctionTemplate>(ReturnNull))
      .ToLocalChecked()
  );
}

NODE_MODULE(returnnull, Init)

