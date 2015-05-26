/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ReturnUtf8String) {
  NanReturnValue(NanNew(*NanUtf8String(args[0])));
}

NAN_METHOD(HeapString) {
  NanUtf8String *s = new NanUtf8String(args[0]);
  v8::Local<v8::String> res = NanNew(**s);
  delete s;
  NanReturnValue(res);
}

NAN_METHOD(EncodeHex) {
  NanReturnValue(NanEncode("hello", 5, Nan::HEX));
}

NAN_METHOD(EncodeUCS2) {
  NanReturnValue(NanEncode("h\0e\0l\0l\0o\0", 10, Nan::UCS2));
}

v8::Persistent<v8::FunctionTemplate> returnUtf8String_persistent;
v8::Persistent<v8::FunctionTemplate> heapString_persistent;
v8::Persistent<v8::FunctionTemplate> encodeHex_persistent;
v8::Persistent<v8::FunctionTemplate> encodeUCS2_persistent;

void Init (v8::Handle<v8::Object> target) {
  v8::Local<v8::FunctionTemplate> returnUtf8String =
    NanNew<v8::FunctionTemplate>(ReturnUtf8String);

  NanAssignPersistent(
    returnUtf8String_persistent
  , returnUtf8String
  );

  target->Set(
      NanNew("returnUtf8String")
    , returnUtf8String->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> heapString =
    NanNew<v8::FunctionTemplate>(HeapString);

  NanAssignPersistent(
    heapString_persistent
  , heapString
  );

  target->Set(
      NanNew("heapString")
    , heapString->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> encodeHex =
    NanNew<v8::FunctionTemplate>(EncodeHex);

  NanAssignPersistent(
    encodeHex_persistent
  , encodeHex
  );

  target->Set(
      NanNew("encodeHex")
    , encodeHex->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> encodeUCS2 =
    NanNew<v8::FunctionTemplate>(EncodeUCS2);

  NanAssignPersistent(
    encodeUCS2_persistent
  , encodeUCS2
  );

  target->Set(
      NanNew("encodeUCS2")
    , encodeUCS2->GetFunction()
  );
}

NODE_MODULE(strings, Init)
