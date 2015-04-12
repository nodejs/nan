/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(NewNumber) {
  NanScope();
  NanReturnValue(NanNew(0.5));
}

NAN_METHOD(NewNegativeInteger) {
  NanScope();
  NanReturnValue(NanNew(-1));
}

NAN_METHOD(NewPositiveInteger) {
  NanScope();
  NanReturnValue(NanNew(1));
}

NAN_METHOD(NewUtf8String) {
  NanScope();
  const char s[] = "strïng";
  NanReturnValue(NanNew(s).ToLocalChecked());
}

NAN_METHOD(NewLatin1String) {
  NanScope();
  const uint8_t s[] = "str\xefng";
  NanReturnValue(NanNew(s).ToLocalChecked());
}

NAN_METHOD(NewUcs2String) {
  NanScope();
  uint16_t s[] = {'s', 't', 'r', 0xef, 'n', 'g', '\0'};
  NanReturnValue(NanNew(s).ToLocalChecked());
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
  NanScope();
  v8::Local<v8::String> ext = NanNew(new ExtString()).ToLocalChecked();
  NanReturnValue(ext);
}

NAN_METHOD(NewExternalAsciiStringResource) {
  NanScope();
  v8::Local<v8::String> ext = NanNew(new ExtAsciiString()).ToLocalChecked();
  NanReturnValue(ext);
}

void Init(v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew("newNumber").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewNumber)->GetFunction()
  );
  NanSet(target
    , NanNew("newNegativeInteger").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewNegativeInteger)->GetFunction()
  );
  NanSet(target
    , NanNew("newPositiveInteger").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewPositiveInteger)->GetFunction()
  );
  NanSet(target
    , NanNew("newUtf8String").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewUtf8String)->GetFunction()
  );
  NanSet(target
    , NanNew("newLatin1String").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewLatin1String)->GetFunction()
  );
  NanSet(target
    , NanNew("newUcs2String").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewUcs2String)->GetFunction()
  );
  NanSet(target
    , NanNew("newExternalStringResource").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewExternalStringResource)->GetFunction()
  );
  NanSet(target
    , NanNew("newExternalAsciiStringResource").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewExternalAsciiStringResource)
    ->GetFunction()
  );
}

NODE_MODULE(morenews, Init)
