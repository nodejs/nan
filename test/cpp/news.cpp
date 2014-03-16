/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <node.h>
#include <nan.h>

NAN_METHOD(NewNumber) {
  NanScope();
  NanReturnValue(NanNew<v8::Number>(0.5));
}
NAN_METHOD(NewNegativeInteger) {
  NanScope();
  NanReturnValue(NanNew<v8::Integer>(-1));
}
NAN_METHOD(NewPositiveInteger) {
  NanScope();
  NanReturnValue(NanNew<v8::Integer>(1));
}
NAN_METHOD(NewInt32FromPositive) {
  NanScope();
  NanReturnValue(NanNew<v8::Int32>(0xFFFFFFFF));
}
NAN_METHOD(NewInt32FromNegative) {
  NanScope();
  NanReturnValue(NanNew<v8::Int32>(-1));
}
NAN_METHOD(NewUint32FromPositive) {
  NanScope();
  NanReturnValue(NanNew<v8::Uint32>(0xFFFFFFFF));
}
NAN_METHOD(NewUint32FromNegative) {
  NanScope();
  NanReturnValue(NanNew<v8::Uint32>(-1));
}
NAN_METHOD(NewUtf8String) {
  NanScope();
  const char s[] = "strïng";
  NanReturnValue(NanNew<v8::String>(s));
}
NAN_METHOD(NewLatin1String) {
  NanScope();
  const uint8_t s[] = "str\xefng";
  NanReturnValue(NanNew<v8::String>(s));
}
NAN_METHOD(NewUcs2String) {
  NanScope();
  uint16_t s[] = {'s', 't', 'r', 0x00ef, 'n', 'g', 0};
  NanReturnValue(NanNew<v8::String>(s));
}

void Init(v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("newNumber")
    , NanNew<v8::FunctionTemplate>(NewNumber)->GetFunction()
  );
  target->Set(
      NanSymbol("newNegativeInteger")
    , NanNew<v8::FunctionTemplate>(NewNegativeInteger)->GetFunction()
  );
  target->Set(
      NanSymbol("newPositiveInteger")
    , NanNew<v8::FunctionTemplate>(NewPositiveInteger)->GetFunction()
  );
  target->Set(
      NanSymbol("newInt32FromPositive")
    , NanNew<v8::FunctionTemplate>(NewInt32FromPositive)->GetFunction()
  );
  target->Set(
      NanSymbol("newInt32FromNegative")
    , NanNew<v8::FunctionTemplate>(NewInt32FromNegative)->GetFunction()
  );
  target->Set(
      NanSymbol("newUint32FromPositive")
    , NanNew<v8::FunctionTemplate>(NewUint32FromPositive)->GetFunction()
  );
  target->Set(
      NanSymbol("newUint32FromNegative")
    , NanNew<v8::FunctionTemplate>(NewUint32FromNegative)->GetFunction()
  );
  target->Set(
      NanSymbol("newUtf8String")
    , NanNew<v8::FunctionTemplate>(NewUtf8String)->GetFunction()
  );
  target->Set(
      NanSymbol("newLatin1String")
    , NanNew<v8::FunctionTemplate>(NewLatin1String)->GetFunction()
  );
  target->Set(
      NanSymbol("newUcs2String")
    , NanNew<v8::FunctionTemplate>(NewUcs2String)->GetFunction()
  );
}

NODE_MODULE(news, Init)
