/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <node.h>
#include <nan.h>

NAN_METHOD(ReturnString) {
  NanScope();

  NanReturnValue(NanNew<v8::String>((const char *)NanFromV8String(args[0])));
}
