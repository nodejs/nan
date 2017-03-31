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
  if (0 == info.Length()) {
    info.GetReturnValue().SetUndefined();
  } else {
    Nan::MaybeLocal<v8::Object> maybe_obj = Nan::To<v8::Object>(info[0]);
    if (maybe_obj.IsEmpty()) {
      info.GetReturnValue().SetUndefined();
    } else {
      Nan::JSON NanJSON;
      v8::Local<v8::Object> obj = maybe_obj.ToLocalChecked();

      if (3 == info.Length()) {
        if (info[2]->IsNumber()) {
          int len = info[2]->IntegerValue();
          len = (len > 10) ? 10 : len;
          len = (len < 0) ? 0 : len;
          Nan::MaybeLocal<v8::String> maybe_gap =
            Nan::New<v8::String>(std::string(len, ' '));

          if (maybe_gap.IsEmpty()) {
            info.GetReturnValue().SetUndefined();
          } else {
            v8::Local<v8::String> gap = maybe_gap.ToLocalChecked();

            Nan::MaybeLocal<v8::String> result =
              NanJSON.Stringify(obj, gap);

            if (result.IsEmpty()) {
              info.GetReturnValue().SetUndefined();
            } else {
              info.GetReturnValue().Set(result.ToLocalChecked());
            }
          }
        } else if (info[2]->IsString()) {
          Nan::MaybeLocal<v8::String> result = NanJSON.Stringify(
            obj,
            Nan::To<v8::String>(info[2]).ToLocalChecked()
          );

          if (result.IsEmpty()) {
            info.GetReturnValue().SetUndefined();
          } else {
            info.GetReturnValue().Set(result.ToLocalChecked());
          }
        } else {
          Nan::MaybeLocal<v8::String> result = NanJSON.Stringify(obj);

          if (result.IsEmpty()) {
            info.GetReturnValue().SetUndefined();
          } else {
            info.GetReturnValue().Set(result.ToLocalChecked());
          }
        }
      } else {
        Nan::MaybeLocal<v8::String> result = NanJSON.Stringify(obj);

        if (result.IsEmpty()) {
          info.GetReturnValue().SetUndefined();
        } else {
          info.GetReturnValue().Set(result.ToLocalChecked());
        }
      }
    }
  }
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("stringify").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(Stringify)->GetFunction()
  );
}

NODE_MODULE(stringify, Init)