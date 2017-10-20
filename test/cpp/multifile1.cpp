/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2017 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include "./multifile2.h"

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(ReturnInt) {
  info.GetReturnValue().Set(1337);
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("r").ToLocalChecked()
    , New<v8::FunctionTemplate>(ReturnString)->GetFunction()
  );
  Set(target
    , New<v8::String>("s").ToLocalChecked()
    , New<v8::FunctionTemplate>(ReturnInt)->GetFunction()
  );
}

NODE_MODULE(multifile, Init)
