/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

NAN_METHOD(ReturnEmptyString) {
  NanScope();
  NanReturnEmptyString();
}

NAN_REGFUNC(Init) {
  exports->Set(
      NanNew<v8::String>("r")
    , NanNew<v8::FunctionTemplate>(ReturnEmptyString)->GetFunction()
  );
}

NAN_MODULE(returnemptystring, Init)

