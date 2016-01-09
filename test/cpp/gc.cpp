/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

static bool prologue_called = false;
static bool epilogue_called = false;

NAN_GC_CALLBACK(gcPrologueCallback) {
  prologue_called = true;
}

NAN_GC_CALLBACK(gcEpilogueCallback) {
  epilogue_called = true;
}

NAN_METHOD(Hook) {
  AddGCPrologueCallback(gcPrologueCallback);
  AddGCEpilogueCallback(gcEpilogueCallback);
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Check) {
  HandleScope scope;
  info.GetReturnValue().Set(
      New(prologue_called && epilogue_called));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("hook").ToLocalChecked()
    , New<v8::FunctionTemplate>(Hook)->GetFunction()
  );
  Set(target
    , New<v8::String>("check").ToLocalChecked()
    , New<v8::FunctionTemplate>(Check)->GetFunction()
  );
}

NODE_MODULE(gc, Init)
