/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(CallHandler) {
  info.GetReturnValue().Set(12);
}

NAN_METHOD(CallHandlerSetter) {
  v8::Local<v8::FunctionTemplate> tpl = New<v8::FunctionTemplate>();
  SetCallHandler(tpl, CallHandler);
  info.GetReturnValue().Set(GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(CallAsFunctionHandler) {
  info.GetReturnValue().Set(15);
}

NAN_METHOD(CallAsFunctionHandlerSetter) {
  v8::Local<v8::ObjectTemplate> tpl = New<v8::ObjectTemplate>();
  SetCallAsFunctionHandler(tpl, CallAsFunctionHandler);
  info.GetReturnValue().Set(NewInstance(tpl).ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New("a").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CallHandlerSetter)).ToLocalChecked()
  );
  Set(target
    , New("b").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CallAsFunctionHandlerSetter))
        .ToLocalChecked()
  );
}

NODE_MODULE(setcallhandler, Init)

