/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(NewNumber) {
  NanReturnValue(NanNew(0.5));
}

NAN_METHOD(NewNegativeInteger) {
  NanReturnValue(NanNew(-1));
}

NAN_METHOD(NewPositiveInteger) {
  NanReturnValue(NanNew(1));
}

NAN_METHOD(NewUtf8String) {
  const char s[] = "strïng";
  NanReturnValue(NanNew(s));
}

NAN_METHOD(NewLatin1String) {
  const uint8_t s[] = "str\xefng";
  NanReturnValue(NanNew(s));
}

NAN_METHOD(NewUcs2String) {
  uint16_t s[] = {'s', 't', 'r', 0xef, 'n', 'g', '\0'};
  NanReturnValue(NanNew(s));
}

static const uint16_t ws[] = {'s', 't', 'r', 0xef, 'n', 'g', '\0'};
static const char s[] = {'s', 't', 'r', 'i', 'n', 'g', '\0'};

class ExtString : public v8::String::ExternalStringResource {
 public:
  ~ExtString() { }
  const uint16_t *data() const { return ws; }
  size_t length() const { return sizeof (ws) / sizeof (*ws) - 1; }
};


class ExtAsciiString : public NanExternalOneByteStringResource {
 public:
  ~ExtAsciiString() { }
  const char *data() const { return s; }
  size_t length() const { return sizeof (s) / sizeof (*s) - 1; }
};

NAN_METHOD(NewExternalStringResource) {
  v8::Local<v8::String> ext = NanNew(new ExtString());
  NanReturnValue(ext);
}

NAN_METHOD(NewExternalAsciiStringResource) {
  v8::Local<v8::String> ext = NanNew(new ExtAsciiString());
  NanReturnValue(ext);
}

void Init(v8::Handle<v8::Object> target) {
  target->Set(
      NanNew("newNumber")
    , NanNew<v8::FunctionTemplate>(NewNumber)->GetFunction()
  );
  target->Set(
      NanNew("newNegativeInteger")
    , NanNew<v8::FunctionTemplate>(NewNegativeInteger)->GetFunction()
  );
  target->Set(
      NanNew("newPositiveInteger")
    , NanNew<v8::FunctionTemplate>(NewPositiveInteger)->GetFunction()
  );
  target->Set(
      NanNew("newUtf8String")
    , NanNew<v8::FunctionTemplate>(NewUtf8String)->GetFunction()
  );
  target->Set(
      NanNew("newLatin1String")
    , NanNew<v8::FunctionTemplate>(NewLatin1String)->GetFunction()
  );
  target->Set(
      NanNew("newUcs2String")
    , NanNew<v8::FunctionTemplate>(NewUcs2String)->GetFunction()
  );
  target->Set(
      NanNew("newExternalStringResource")
    , NanNew<v8::FunctionTemplate>(NewExternalStringResource)->GetFunction()
  );
  target->Set(
      NanNew("newExternalAsciiStringResource")
    , NanNew<v8::FunctionTemplate>(NewExternalAsciiStringResource)
    ->GetFunction()
  );
}

NODE_MODULE(morenews, Init)
