/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <node.h>
#include <nan.h>

static int magic = 1337;

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
  uint16_t s[] = {'s', 't', 'r', 0xef, 'n', 'g', '\0'};
  NanReturnValue(NanNew<v8::String>(s));
}
NAN_METHOD(NewRegExp) {
  NanScope();
  NanReturnValue(NanNew<v8::RegExp>(NanNew<v8::String>("foo"), v8::RegExp::kNone));
}
NAN_METHOD(NewStringObject) {
  NanScope();
  NanReturnValue(NanNew<v8::StringObject>(NanNew<v8::String>("foo")));
}
NAN_METHOD(NewNumberObject) {
  NanScope();
  NanReturnValue(NanNew<v8::NumberObject>(0.5));
}
NAN_METHOD(NewBooleanObject) {
  NanScope();
  NanReturnValue(NanNew<v8::BooleanObject>(true));
}
NAN_METHOD(NewExternal) {
  NanScope();
  v8::Local<v8::External> ext = NanNew<v8::External>(&magic);
  assert(*static_cast<int *>(ext->Value()) == 1337);
  NanReturnValue(NanNew<v8::String>("passed"));
}

static const uint16_t ws[] = {'s', 't', 'r', 0xef, 'n', 'g', '\0'};
static const char s[] = {'s', 't', 'r', 'i', 'n', 'g', '\0'};

class ExtString : public v8::String::ExternalStringResource {
 public:
  ~ExtString() { }
  const uint16_t *data() const { return ws; }
  size_t length() const { return sizeof (ws) / sizeof (*ws) - 1; };
};


class ExtAsciiString : public v8::String::ExternalAsciiStringResource {
 public:
  ~ExtAsciiString() { }
  const char *data() const { return s; }
  size_t length() const { return sizeof (s) / sizeof (*s) - 1; };
};

NAN_METHOD(NewExternalStringResource) {
  NanScope();
  v8::Local<v8::String> ext = NanNew(new ExtString());
  NanReturnValue(ext);
}

NAN_METHOD(NewExternalAsciiStringResource) {
  NanScope();
  v8::Local<v8::String> ext = NanNew(new ExtAsciiString());
  NanReturnValue(ext);
}

NAN_METHOD(NewSignature) {
  NanScope();
  v8::Local<v8::FunctionTemplate> tmpl = NanNew<v8::FunctionTemplate>(NewSignature);
  v8::Local<v8::Signature> sig = NanNew<v8::Signature>(tmpl, 1, &tmpl);
  tmpl = NanNew<v8::FunctionTemplate>(NewSignature, v8::Handle<v8::Value>(), sig);
  NanReturnValue(NanNew<v8::String>("string"));
}

NAN_METHOD(NewScript) {
  NanScope();
  v8::Local<NanUnboundScript> script = NanNew<NanUnboundScript>(NanNew<v8::String>("2+4"));
  NanReturnValue(NanRunScript(script)->ToInt32());
}

NAN_METHOD(NewScript2) {
  NanScope();
  v8::ScriptOrigin origin(NanNew<v8::String>("x"));
  v8::Local<NanUnboundScript> script = NanNew<NanUnboundScript>(NanNew<v8::String>("2+4"), origin);
  NanReturnValue(NanRunScript(script)->ToInt32());
}

NAN_METHOD(CompileScript) {
  NanScope();
  v8::Local<NanBoundScript> script = NanCompileScript(NanNew<v8::String>("2+4"));
  NanReturnValue(NanRunScript(script)->ToInt32());
}

NAN_METHOD(CompileScript2) {
  NanScope();
  v8::ScriptOrigin origin(NanNew<v8::String>("x"));
  v8::Local<NanBoundScript> script = NanCompileScript(NanNew<v8::String>("2+4"), origin);
  NanReturnValue(NanRunScript(script)->ToInt32());
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
  target->Set(
      NanSymbol("newRegExp")
    , NanNew<v8::FunctionTemplate>(NewRegExp)->GetFunction()
  );
  target->Set(
      NanSymbol("newStringObject")
    , NanNew<v8::FunctionTemplate>(NewStringObject)->GetFunction()
  );
  target->Set(
      NanSymbol("newNumberObject")
    , NanNew<v8::FunctionTemplate>(NewNumberObject)->GetFunction()
  );
  target->Set(
      NanSymbol("newBooleanObject")
    , NanNew<v8::FunctionTemplate>(NewBooleanObject)->GetFunction()
  );
  target->Set(
      NanSymbol("newExternal")
    , NanNew<v8::FunctionTemplate>(NewExternal)->GetFunction()
  );
  target->Set(
      NanSymbol("newExternalStringResource")
    , NanNew<v8::FunctionTemplate>(NewExternalStringResource)->GetFunction()
  );
  target->Set(
      NanSymbol("newExternalAsciiStringResource")
    , NanNew<v8::FunctionTemplate>(NewExternalAsciiStringResource)->GetFunction()
  );
  target->Set(
      NanSymbol("newSignature")
    , NanNew<v8::FunctionTemplate>(NewSignature)->GetFunction()
  );
  target->Set(
      NanSymbol("newScript")
    , NanNew<v8::FunctionTemplate>(NewScript)->GetFunction()
  );
  target->Set(
      NanSymbol("newScript2")
    , NanNew<v8::FunctionTemplate>(NewScript2)->GetFunction()
  );
  target->Set(
      NanSymbol("compileScript")
    , NanNew<v8::FunctionTemplate>(CompileScript)->GetFunction()
  );
  target->Set(
      NanSymbol("compileScript2")
    , NanNew<v8::FunctionTemplate>(CompileScript2)->GetFunction()
  );
}

NODE_MODULE(news, Init)
