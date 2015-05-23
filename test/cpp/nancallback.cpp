/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(GlobalContext) {
  NanCallback(info[0].As<v8::Function>()).Call(0, NULL);
}

NAN_METHOD(SpecificContext) {
  NanCallback cb(info[0].As<v8::Function>());
  cb.Call(NanGetCurrentContext()->Global(), 0, NULL);
}

NAN_METHOD(CustomReceiver) {
  NanCallback cb(info[0].As<v8::Function>());
  cb.Call(info[1].As<v8::Object>(), 0, NULL);
}

NAN_METHOD(CompareCallbacks) {
  NanCallback cb1(info[0].As<v8::Function>());
  NanCallback cb2(info[1].As<v8::Function>());
  NanCallback cb3(info[2].As<v8::Function>());

  info.GetReturnValue().Set(NanNew<v8::Boolean>(cb1 == cb2 && cb1 != cb3));
}

NAN_METHOD(CallDirect) {
  NanCallback cb(info[0].As<v8::Function>());
  (*cb)->Call(NanGetCurrentContext()->Global(), 0, NULL);
}

NAN_METHOD(CallAsFunction) {
  NanCallback(info[0].As<v8::Function>())();
}

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("globalContext").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(GlobalContext)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("specificContext").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(SpecificContext)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("customReceiver").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(CustomReceiver)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("compareCallbacks").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(CompareCallbacks)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("callDirect").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(CallDirect)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("callAsFunction").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(CallAsFunction)->GetFunction()
  );
}

NODE_MODULE(nancallback, Init)
