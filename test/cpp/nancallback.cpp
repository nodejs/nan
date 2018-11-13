/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(GlobalContext) {
  AsyncResource resource("nan:test.nancallback");
  Callback(To<v8::Function>(info[0]).ToLocalChecked()).Call(0, NULL, &resource);
}

NAN_METHOD(SpecificContext) {
  AsyncResource resource("nan:test.nancallback");
  Callback cb(To<v8::Function>(info[0]).ToLocalChecked());
  cb.Call(GetCurrentContext()->Global(), 0, NULL, &resource);
}

NAN_METHOD(CustomReceiver) {
  AsyncResource resource("nan:test.nancallback");
  Callback cb(To<v8::Function>(info[0]).ToLocalChecked());
  cb.Call(To<v8::Object>(info[1]).ToLocalChecked(), 0, NULL, &resource);
}

NAN_METHOD(CompareCallbacks) {
  Callback cb1(To<v8::Function>(info[0]).ToLocalChecked());
  Callback cb2(To<v8::Function>(info[1]).ToLocalChecked());
  Callback cb3(To<v8::Function>(info[2]).ToLocalChecked());

  info.GetReturnValue().Set(New<v8::Boolean>(cb1 == cb2 && cb1 != cb3));
}

NAN_METHOD(CallDirect) {
  Callback cb(To<v8::Function>(info[0]).ToLocalChecked());
  Call(*cb, GetCurrentContext()->Global(), 0, NULL);
}

NAN_METHOD(CallAsFunction) {
  AsyncResource resource("nan:test.nancallback");
  Callback(To<v8::Function>(info[0]).ToLocalChecked())(&resource);
}

NAN_METHOD(ResetUnset) {
  Callback callback;
  callback.Reset();
  info.GetReturnValue().Set(callback.IsEmpty());
}

NAN_METHOD(ResetSet) {
  Callback callback(To<v8::Function>(info[0]).ToLocalChecked());
  callback.Reset();
  info.GetReturnValue().Set(callback.IsEmpty());
}

NAN_METHOD(CallRetval) {
  AsyncResource resource("nan:test.nancallback");
  Callback callback(To<v8::Function>(info[0]).ToLocalChecked());
  v8::Local<v8::Value> result =
    callback.Call(0, NULL, &resource).ToLocalChecked();
  if (result->IsNumber()) {
    info.GetReturnValue().Set(Nan::True());
  } else {
    info.GetReturnValue().Set(Nan::False());
  }
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("globalContext").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(GlobalContext)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("specificContext").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(SpecificContext)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("customReceiver").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CustomReceiver)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("compareCallbacks").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CompareCallbacks)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("callDirect").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CallDirect)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("callAsFunction").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CallAsFunction)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("resetUnset").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ResetUnset)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("resetSet").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ResetSet)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("callRetval").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CallRetval)).ToLocalChecked()
  );
}

NODE_MODULE(nancallback, Init)
