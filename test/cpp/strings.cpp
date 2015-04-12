/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(ReturnAsciiString) {
  NanScope();
  NanReturnValue(NanNew(*NanAsciiString(args[0])).ToLocalChecked());
}

NAN_METHOD(ReturnUtf8String) {
  NanScope();
  NanReturnValue(NanNew(*NanUtf8String(args[0])).ToLocalChecked());
}

NAN_METHOD(ReturnUcs2String) {
  NanScope();
  NanReturnValue(NanNew(*NanUcs2String(args[0])).ToLocalChecked());
}

NAN_METHOD(HeapString) {
  NanScope();
  NanUcs2String *s = new NanUcs2String(args[0]);
  v8::Local<v8::String> res = NanNew(**s).ToLocalChecked();
  delete s;
  NanReturnValue(res);
}

NAN_METHOD(EncodeHex) {
  NanScope();
  NanReturnValue(NanEncode("hello", 5, Nan::HEX));
}

NAN_METHOD(EncodeUCS2) {
  NanScope();
  NanReturnValue(NanEncode("h\0e\0l\0l\0o\0", 10, Nan::UCS2));
}

v8::Persistent<v8::FunctionTemplate> returnAsciiString_persistent;
v8::Persistent<v8::FunctionTemplate> returnUtf8String_persistent;
v8::Persistent<v8::FunctionTemplate> returnUcs2String_persistent;
v8::Persistent<v8::FunctionTemplate> heapString_persistent;
v8::Persistent<v8::FunctionTemplate> encodeHex_persistent;
v8::Persistent<v8::FunctionTemplate> encodeUCS2_persistent;

void Init (v8::Handle<v8::Object> target) {
  NanScope();

  v8::Local<v8::FunctionTemplate> returnAsciiString =
    NanNew<v8::FunctionTemplate>(ReturnAsciiString);

  NanAssignPersistent(
    returnAsciiString_persistent
  , returnAsciiString
  );

  NanSet(target
    , NanNew("returnAsciiString").ToLocalChecked()
    , returnAsciiString->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> returnUtf8String =
    NanNew<v8::FunctionTemplate>(ReturnUtf8String);

  NanAssignPersistent(
    returnUtf8String_persistent
  , returnUtf8String
  );

  NanSet(target
    , NanNew("returnUtf8String").ToLocalChecked()
    , returnUtf8String->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> returnUcs2String =
    NanNew<v8::FunctionTemplate>(ReturnUcs2String);

  NanAssignPersistent(
    returnUcs2String_persistent
  , returnUcs2String
  );

  NanSet(target
    , NanNew("returnUcs2String").ToLocalChecked()
    , returnUcs2String->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> heapString =
    NanNew<v8::FunctionTemplate>(HeapString);

  NanAssignPersistent(
    heapString_persistent
  , heapString
  );

  NanSet(target
    , NanNew("heapString").ToLocalChecked()
    , heapString->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> encodeHex =
    NanNew<v8::FunctionTemplate>(EncodeHex);

  NanAssignPersistent(
    encodeHex_persistent
  , encodeHex
  );

  NanSet(target
    , NanNew("encodeHex").ToLocalChecked()
    , encodeHex->GetFunction()
  );

  v8::Local<v8::FunctionTemplate> encodeUCS2 =
    NanNew<v8::FunctionTemplate>(EncodeUCS2);

  NanAssignPersistent(
    encodeUCS2_persistent
  , encodeUCS2
  );

  NanSet(target
    , NanNew("encodeUCS2").ToLocalChecked()
    , encodeUCS2->GetFunction()
  );
}

NODE_MODULE(strings, Init)
