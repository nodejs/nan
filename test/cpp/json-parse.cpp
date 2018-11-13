/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(Parse) {
  Nan::JSON NanJSON;

  Nan::MaybeLocal<v8::String> inp = Nan::To<v8::String>(info[0]);

  if (!inp.IsEmpty()) {
    Nan::MaybeLocal<v8::Value> result = NanJSON.Parse(
      inp.ToLocalChecked()
    );

    if (!result.IsEmpty()) {
      info.GetReturnValue().Set(result.ToLocalChecked());
    }
  }
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("parse").ToLocalChecked()
    , Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Parse)).ToLocalChecked()
  );
}

NODE_MODULE(parse, Init)
