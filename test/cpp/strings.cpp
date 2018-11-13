/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(ReturnUtf8String) {
  info.GetReturnValue().Set(New(*Utf8String(info[0])).ToLocalChecked());
}

NAN_METHOD(HeapString) {
  Utf8String *s = new Utf8String(info[0]);
  v8::Local<v8::String> res = New(**s).ToLocalChecked();
  delete s;
  info.GetReturnValue().Set(res);
}

NAN_METHOD(EncodeHex) {
  info.GetReturnValue().Set(Encode("hello", 5, HEX));
}

NAN_METHOD(EncodeUCS2) {
  info.GetReturnValue().Set(Encode("h\0e\0l\0l\0o\0", 10, UCS2));
}

Persistent<v8::FunctionTemplate> returnUtf8String_persistent;
Persistent<v8::FunctionTemplate> heapString_persistent;
Persistent<v8::FunctionTemplate> encodeHex_persistent;
Persistent<v8::FunctionTemplate> encodeUCS2_persistent;

NAN_MODULE_INIT(Init) {
  v8::Local<v8::FunctionTemplate> returnUtf8String =
    New<v8::FunctionTemplate>(ReturnUtf8String);

  returnUtf8String_persistent.Reset(returnUtf8String);

  Set(target
    , New("returnUtf8String").ToLocalChecked()
    , GetFunction(returnUtf8String).ToLocalChecked()
  ).FromJust();

  v8::Local<v8::FunctionTemplate> heapString =
    New<v8::FunctionTemplate>(HeapString);

  heapString_persistent.Reset(heapString);

  Set(target
    , New("heapString").ToLocalChecked()
    , GetFunction(heapString).ToLocalChecked()
  ).FromJust();

  v8::Local<v8::FunctionTemplate> encodeHex =
    New<v8::FunctionTemplate>(EncodeHex);

  encodeHex_persistent.Reset(encodeHex);

  Set(target
    , New("encodeHex").ToLocalChecked()
    , GetFunction(encodeHex).ToLocalChecked()
  ).FromJust();

  v8::Local<v8::FunctionTemplate> encodeUCS2 =
    New<v8::FunctionTemplate>(EncodeUCS2);

  encodeUCS2_persistent.Reset(encodeUCS2);

  Set(target
    , New("encodeUCS2").ToLocalChecked()
    , GetFunction(encodeUCS2).ToLocalChecked()
  ).FromJust();
}

NODE_MODULE(strings, Init)
