/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2017 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <string>

NAN_METHOD(Stringify) {
  if (3 == info.Length()) {
    if (info[2]->IsNumber()) {
      int len = info[2]->IntegerValue();
      len = (len > 10) ? 10 : len;
      len = (len < 0) ? 0 : len;
      v8::Local<v8::String> gap =
        Nan::New<v8::String>(std::string(len, ' ')).ToLocalChecked();
      info.GetReturnValue().Set(
        Nan::JSON::Stringify(info[0]->ToObject(), gap).ToLocalChecked()
      );
    } else if (info[2]->IsString()) {
      info.GetReturnValue().Set(
        Nan::JSON::Stringify(info[0]->ToObject(),
          Nan::To<v8::String>(info[2]).ToLocalChecked()
        ).ToLocalChecked()
      );
    } else {
      info.GetReturnValue().Set(
        Nan::JSON::Stringify(info[0]->ToObject()).ToLocalChecked()
      );
    }
  } else {
    info.GetReturnValue().Set(
      Nan::JSON::Stringify(info[0]->ToObject()).ToLocalChecked()
    );
  }
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("stringify").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(Stringify)->GetFunction()
  );
}

NODE_MODULE(stringify, Init)
