/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

static v8::Persistent<v8::Function> callback;

NAN_GC_CALLBACK(gcPrologueCallback) {
  v8::Local<v8::Value> argv[] =
      {NanNew<v8::String>("prologue").ToLocalChecked()};
  NanMakeCallback(NanGetCurrentContext()->Global(), NanNew(callback), 1, argv);
}

NAN_GC_CALLBACK(gcEpilogueCallback) {
  v8::Local<v8::Value> argv[] =
      {NanNew<v8::String>("epilogue").ToLocalChecked()};
  NanMakeCallback(NanGetCurrentContext()->Global(), NanNew(callback), 1, argv);
}

NAN_METHOD(Hook) {
  NanScope();
  NanAssignPersistent(callback, args[0].As<v8::Function>());
  NanAddGCPrologueCallback(gcPrologueCallback);
  NanAddGCEpilogueCallback(gcEpilogueCallback);
  NanReturnValue(args.Holder());
}

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("hook").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(Hook)->GetFunction()
  );
}

NODE_MODULE(gc, Init)
