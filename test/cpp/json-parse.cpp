/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2017 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(Parse) {
  info.GetReturnValue().Set(
    Nan::JSON::Parse(info[0]->ToString()).ToLocalChecked()
  );
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("parse").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(Parse)->GetFunction()
  );
}

NODE_MODULE(parse, Init)
