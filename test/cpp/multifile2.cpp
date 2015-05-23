/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ReturnString) {
  v8::Local<v8::String> s = NanNew(*NanUtf8String(info[0])).ToLocalChecked();
  info.GetReturnValue().Set(s);
}
