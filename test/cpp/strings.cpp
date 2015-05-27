/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(ReturnUtf8String) {
  info.GetReturnValue().Set(NanNew(*NanUtf8String(info[0])).ToLocalChecked());
}

NAN_METHOD(HeapString) {
  NanUtf8String *s = new NanUtf8String(info[0]);
  v8::Local<v8::String> res = NanNew(**s).ToLocalChecked();
  delete s;
  info.GetReturnValue().Set(res);
}

NAN_METHOD(EncodeHex) {
  info.GetReturnValue().Set(NanEncode("hello", 5, HEX));
}

NAN_METHOD(EncodeUCS2) {
  info.GetReturnValue().Set(NanEncode("h\0e\0l\0l\0o\0", 10, UCS2));
}

NanPersistent<v8::FunctionTemplate> returnUtf8String_persistent;
NanPersistent<v8::FunctionTemplate> heapString_persistent;
NanPersistent<v8::FunctionTemplate> encodeHex_persistent;
NanPersistent<v8::FunctionTemplate> encodeUCS2_persistent;

void Init (v8::Handle<v8::Object> target) {
  v8::Local<v8::FunctionTemplate> returnUtf8String =
    NanNew<v8::FunctionTemplate>(ReturnUtf8String);

  returnUtf8String_persistent.Reset(returnUtf8String);

  target->Set(
      NanNew("returnUtf8String").ToLocalChecked()
    , returnUtf8String->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> heapString =
    NanNew<v8::FunctionTemplate>(HeapString);

  heapString_persistent.Reset(heapString);

  target->Set(
      NanNew("heapString").ToLocalChecked()
    , heapString->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> encodeHex =
    NanNew<v8::FunctionTemplate>(EncodeHex);

  encodeHex_persistent.Reset(encodeHex);

  NanSet(target
    , NanNew("encodeHex").ToLocalChecked()
    , encodeHex->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> encodeUCS2 =
    NanNew<v8::FunctionTemplate>(EncodeUCS2);

  encodeUCS2_persistent.Reset(encodeUCS2);

  NanSet(target
    , NanNew("encodeUCS2").ToLocalChecked()
    , encodeUCS2->GetFunction()
  );
}

NODE_MODULE(strings, Init)
