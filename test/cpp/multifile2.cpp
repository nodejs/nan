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
  char *buf = NanCString(args[0], NULL);
  v8::Local<v8::String> s = NanNew<v8::String>(buf);
  delete[] buf;
  NanReturnValue(s);
}
