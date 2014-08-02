/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

NAN_METHOD(ReturnString) {
  NanScope();
  v8::Local<v8::String> s = NanNew<v8::String>(*NanUtf8String(args[0]));
  NanReturnValue(s);
}
