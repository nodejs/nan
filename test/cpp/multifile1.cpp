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

void Init (v8::Handle<v8::Object> target) {
  Set(target
    , New<v8::String>("r").ToLocalChecked()
    , New<v8::FunctionTemplate>(ReturnString)->GetFunction()
  );
}

NODE_MODULE(multifile, Init)
