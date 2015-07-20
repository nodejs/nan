/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include "./multifile2.h"

using namespace Nan;  // NOLINT(build/namespaces)

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("r").ToLocalChecked()
    , New<v8::FunctionTemplate>(ReturnString)->GetFunction()
  );
}

NODE_MODULE(multifile, Init)
