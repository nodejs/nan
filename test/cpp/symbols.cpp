/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

NAN_REGFUNC(Init) {
  exports->Set(NanNew<v8::String>("key"), NanNew<v8::String>("a property"));
}

NAN_MODULE(symbols, Init)
