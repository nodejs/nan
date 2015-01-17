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
  NanReturnValue(NanNew(*NanAsciiString(args[0])));
}

NAN_METHOD(ReturnUtf8String) {
  NanScope();
  NanReturnValue(NanNew(*NanUtf8String(args[0])));
}

NAN_METHOD(ReturnUcs2String) {
  NanScope();
  NanReturnValue(NanNew(*NanUcs2String(args[0])));
}

NAN_METHOD(HeapString) {
  NanScope();
  NanUcs2String *s = new NanUcs2String(args[0]);
  v8::Local<v8::String> res = NanNew(**s);
  delete s;
  NanReturnValue(res);
}

v8::Persistent<v8::FunctionTemplate> returnAsciiString_persistent;
v8::Persistent<v8::FunctionTemplate> returnUtf8String_persistent;
v8::Persistent<v8::FunctionTemplate> returnUcs2String_persistent;
v8::Persistent<v8::FunctionTemplate> heapString_persistent;

void Init (v8::Handle<v8::Object> target) {
  NanScope();

  v8::Local<v8::FunctionTemplate> returnAsciiString =
    NanNew<v8::FunctionTemplate>(ReturnAsciiString);

  NanAssignPersistent(
    returnAsciiString_persistent
  , returnAsciiString
  );

  target->Set(
      NanNew("returnAsciiString")
    , returnAsciiString->GetFunction()
  );

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

  v8::Local<v8::FunctionTemplate> returnUcs2String =
    NanNew<v8::FunctionTemplate>(ReturnUcs2String);

  NanAssignPersistent(
    returnUcs2String_persistent
  , returnUcs2String
  );

  target->Set(
      NanNew("returnUcs2String")
    , returnUcs2String->GetFunction()
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
}

NODE_MODULE(strings, Init)
