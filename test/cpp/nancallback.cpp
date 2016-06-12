/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(GlobalContext) {
  Callback(info[0].As<v8::Function>()).Call(0, NULL);
}

NAN_METHOD(SpecificContext) {
  Callback cb(info[0].As<v8::Function>());
  cb.Call(GetCurrentContext()->Global(), 0, NULL);
}

NAN_METHOD(CustomReceiver) {
  Callback cb(info[0].As<v8::Function>());
  cb.Call(info[1].As<v8::Object>(), 0, NULL);
}

NAN_METHOD(CompareCallbacks) {
  Callback cb1(info[0].As<v8::Function>());
  Callback cb2(info[1].As<v8::Function>());
  Callback cb3(info[2].As<v8::Function>());

  info.GetReturnValue().Set(New<v8::Boolean>(cb1 == cb2 && cb1 != cb3));
}

NAN_METHOD(CallDirect) {
  Callback cb(info[0].As<v8::Function>());
  (*cb)->Call(GetCurrentContext()->Global(), 0, NULL);
}

NAN_METHOD(CallAsFunction) {
  Callback(info[0].As<v8::Function>())();
}

NAN_METHOD(ResetUnset) {
  Callback callback;
  callback.Reset();
  info.GetReturnValue().Set(callback.IsEmpty());
}

NAN_METHOD(ResetSet) {
  Callback callback(info[0].As<v8::Function>());
  callback.Reset();
  info.GetReturnValue().Set(callback.IsEmpty());
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("globalContext").ToLocalChecked()
    , New<v8::FunctionTemplate>(GlobalContext)->GetFunction()
  );
  Set(target
    , New<v8::String>("specificContext").ToLocalChecked()
    , New<v8::FunctionTemplate>(SpecificContext)->GetFunction()
  );
  Set(target
    , New<v8::String>("customReceiver").ToLocalChecked()
    , New<v8::FunctionTemplate>(CustomReceiver)->GetFunction()
  );
  Set(target
    , New<v8::String>("compareCallbacks").ToLocalChecked()
    , New<v8::FunctionTemplate>(CompareCallbacks)->GetFunction()
  );
  Set(target
    , New<v8::String>("callDirect").ToLocalChecked()
    , New<v8::FunctionTemplate>(CallDirect)->GetFunction()
  );
  Set(target
    , New<v8::String>("callAsFunction").ToLocalChecked()
    , New<v8::FunctionTemplate>(CallAsFunction)->GetFunction()
  );
  Set(target
    , New<v8::String>("resetUnset").ToLocalChecked()
    , New<v8::FunctionTemplate>(ResetUnset)->GetFunction()
  );
  Set(target
    , New<v8::String>("resetSet").ToLocalChecked()
    , New<v8::FunctionTemplate>(ResetSet)->GetFunction()
  );
}

NODE_MODULE(nancallback, Init)
