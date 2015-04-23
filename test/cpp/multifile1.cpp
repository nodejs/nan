/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include "./multifile2.h"

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("r").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(ReturnString)->GetFunction()
  );
}

NODE_MODULE(multifile, Init)
