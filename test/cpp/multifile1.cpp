/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>
#include "./multifile2.h"

NAN_REGFUNC(Init) {
  exports->Set(
      NanNew<v8::String>("r")
    , NanNew<v8::FunctionTemplate>(ReturnString)->GetFunction()
  );
}

NAN_MODULE(multifile, Init)
