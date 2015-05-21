/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(EncodeHex) {
  NanReturnValue(NanEncode("hello", 5, Nan::HEX));
}

NAN_METHOD(EncodeUCS2) {
  NanReturnValue(NanEncode("h\0e\0l\0l\0o\0", 10, Nan::UCS2));
}

NanPersistent<v8::FunctionTemplate> encodeHex_persistent;
NanPersistent<v8::FunctionTemplate> encodeUCS2_persistent;

void Init (v8::Handle<v8::Object> target) {
  v8::Local<v8::FunctionTemplate> encodeHex =
    NanNew<v8::FunctionTemplate>(EncodeHex);

  encodeHex_persistent.Reset(encodeHex);

  target->Set(
      NanNew("encodeHex")
    , encodeHex->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> encodeUCS2 =
    NanNew<v8::FunctionTemplate>(EncodeUCS2);

  encodeUCS2_persistent.Reset(encodeUCS2);

  target->Set(
      NanNew("encodeUCS2")
    , encodeUCS2->GetFunction()
  );
}

NODE_MODULE(strings, Init)
