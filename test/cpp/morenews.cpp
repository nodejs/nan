/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(NewNumber) {
  info.GetReturnValue().Set(New(0.5));
}

NAN_METHOD(NewNegativeInteger) {
  info.GetReturnValue().Set(New(-1));
}

NAN_METHOD(NewPositiveInteger) {
  info.GetReturnValue().Set(New(1));
}

NAN_METHOD(NewUtf8String) {
  const char s[] = "strïng";
  info.GetReturnValue().Set(New(s).ToLocalChecked());
}

NAN_METHOD(NewLatin1String) {
  const uint8_t s[] = "str\xefng";
  info.GetReturnValue().Set(NewOneByteString(s).ToLocalChecked());
}

NAN_METHOD(NewUcs2String) {
  uint16_t s[] = {'s', 't', 'r', 0xef, 'n', 'g', '\0'};
  info.GetReturnValue().Set(New(s).ToLocalChecked());
}

static const uint16_t ws[] = {'s', 't', 'r', 0xef, 'n', 'g', '\0'};
static const char s[] = {'s', 't', 'r', 'i', 'n', 'g', '\0'};

class ExtString : public v8::String::ExternalStringResource {
 public:
  ~ExtString() { }
  const uint16_t *data() const { return ws; }
  size_t length() const { return sizeof (ws) / sizeof (*ws) - 1; }
};


class ExtAsciiString : public ExternalOneByteStringResource {
 public:
  ~ExtAsciiString() { }
  const char *data() const { return s; }
  size_t length() const { return sizeof (s) / sizeof (*s) - 1; }
};

NAN_METHOD(NewExternalStringResource) {
  v8::Local<v8::String> ext = New(new ExtString()).ToLocalChecked();
  info.GetReturnValue().Set(ext);
}

NAN_METHOD(NewExternalAsciiStringResource) {
  v8::Local<v8::String> ext = New(new ExtAsciiString()).ToLocalChecked();
  info.GetReturnValue().Set(ext);
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New("newNumber").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewNumber)).ToLocalChecked()
  );
  Set(target
    , New("newNegativeInteger").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewNegativeInteger))
        .ToLocalChecked()
  );
  Set(target
    , New("newPositiveInteger").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewPositiveInteger))
        .ToLocalChecked()
  );
  Set(target
    , New("newUtf8String").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewUtf8String)).ToLocalChecked()
  );
  Set(target
    , New("newLatin1String").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewLatin1String)).ToLocalChecked()
  );
  Set(target
    , New("newUcs2String").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewUcs2String)).ToLocalChecked()
  );
  Set(target
    , New("newExternalStringResource").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewExternalStringResource))
        .ToLocalChecked()
  );
  Set(target
    , New("newExternalAsciiStringResource").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewExternalAsciiStringResource))
        .ToLocalChecked()
  );
}

NODE_MODULE(morenews, Init)
