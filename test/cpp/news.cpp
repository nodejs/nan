/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#if defined(_MSC_VER)
# pragma warning( push )
# pragma warning( disable : 4530 )
# include <string>
# pragma warning( pop )
#else
# include <string>
#endif

using namespace Nan;  // NOLINT(build/namespaces)

static int magic = 1337;

NAN_METHOD(NewNumber) {
  info.GetReturnValue().Set(New<v8::Number>(0.5));
}

NAN_METHOD(NewNegativeInteger) {
  info.GetReturnValue().Set(New<v8::Integer>(-1));
}

NAN_METHOD(NewPositiveInteger) {
  info.GetReturnValue().Set(New<v8::Integer>(1));
}

NAN_METHOD(NewUnsignedInteger) {
  info.GetReturnValue().Set(New<v8::Integer>(0xFFFFFFFFu));
}

NAN_METHOD(NewInt32FromPositive) {
  info.GetReturnValue().Set(New<v8::Int32>(0xFFFFFFFF));
}

NAN_METHOD(NewInt32FromNegative) {
  info.GetReturnValue().Set(New<v8::Int32>(-1));
}

NAN_METHOD(NewUint32FromPositive) {
  info.GetReturnValue().Set(New<v8::Uint32>(0xFFFFFFFF));
}

NAN_METHOD(NewUint32FromNegative) {
  info.GetReturnValue().Set(New<v8::Uint32>(-1));
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
  const uint16_t s[] = {'s', 't', 'r', 0xef, 'n', 'g', '\0'};
  info.GetReturnValue().Set(New(s).ToLocalChecked());
}

NAN_METHOD(NewStdString) {
  const std::string s = "strïng";
  info.GetReturnValue().Set(New<v8::String>(s).ToLocalChecked());
}

NAN_METHOD(NewRegExp) {
  info.GetReturnValue().Set(
      New<v8::RegExp>(New("foo").ToLocalChecked()
    , v8::RegExp::kNone).ToLocalChecked());
}

NAN_METHOD(NewStringObject) {
  info.GetReturnValue().Set(
      New<v8::StringObject>(New("foo").ToLocalChecked()));
}

NAN_METHOD(NewNumberObject) {
  info.GetReturnValue().Set(New<v8::NumberObject>(0.5));
}

NAN_METHOD(NewBooleanObject) {
  info.GetReturnValue().Set(New<v8::BooleanObject>(true));
}

NAN_METHOD(NewExternal) {
  v8::Local<v8::External> ext = New<v8::External>(&magic);
  assert(*static_cast<int *>(ext->Value()) == 1337);
  info.GetReturnValue().Set(New("passed").ToLocalChecked());
}

NAN_METHOD(NewSignature) {
  v8::Local<v8::FunctionTemplate> tmpl =
      New<v8::FunctionTemplate>(NewSignature);
  v8::Local<v8::Signature> sig = New<v8::Signature>(tmpl);
  tmpl = New<v8::FunctionTemplate>(
      NewSignature, v8::Local<v8::Value>(), sig);
  info.GetReturnValue().Set(New("string").ToLocalChecked());
}

NAN_METHOD(NewScript) {
  v8::Local<UnboundScript> script =
      New<UnboundScript>(New("2+4").ToLocalChecked()).ToLocalChecked();
  info.GetReturnValue().Set(
      To<v8::Int32>(RunScript(script).ToLocalChecked()).ToLocalChecked());
}

NAN_METHOD(NewScript2) {
  v8::ScriptOrigin origin(New<v8::String>("x").ToLocalChecked());
  v8::Local<UnboundScript> script =
      New<UnboundScript>(
          New("2+4").ToLocalChecked()
        , origin).ToLocalChecked();
  info.GetReturnValue().Set(
      To<v8::Int32>(RunScript(script).ToLocalChecked()).ToLocalChecked());
}

NAN_METHOD(CompileScript) {
  v8::Local<BoundScript> script =
      CompileScript(New("2+4").ToLocalChecked()).ToLocalChecked();
  info.GetReturnValue().Set(
      To<v8::Int32>(RunScript(script).ToLocalChecked()).ToLocalChecked());
}

NAN_METHOD(CompileScript2) {
  v8::ScriptOrigin origin(New<v8::String>("x").ToLocalChecked());
  v8::Local<BoundScript> script =
      CompileScript(New("2+4").ToLocalChecked(), origin).ToLocalChecked();
  info.GetReturnValue().Set(
      To<v8::Int32>(RunScript(script).ToLocalChecked()).ToLocalChecked());
}

NAN_METHOD(NewDate) {
  info.GetReturnValue().Set(New<v8::Date>(1337).ToLocalChecked());
}

NAN_METHOD(NewArray) {
  info.GetReturnValue().Set(New<v8::Array>());
}

NAN_METHOD(NewBoolean) {
  info.GetReturnValue().Set(New<v8::Boolean>(true));
}

// #212
NAN_METHOD(NewBoolean2) {
#if defined(_MSC_VER)
# pragma warning( push )
# pragma warning( disable : 4800 )
#endif
  info.GetReturnValue().Set(New<v8::Boolean>(1));
#if defined(_MSC_VER)
# pragma warning( pop )
#endif
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("newNumber").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewNumber)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newNegativeInteger").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewNegativeInteger))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newPositiveInteger").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewPositiveInteger))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newUnsignedInteger").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewUnsignedInteger))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newInt32FromPositive").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewInt32FromPositive))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newInt32FromNegative").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewInt32FromNegative))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newUint32FromPositive").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewUint32FromPositive))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newUint32FromNegative").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewUint32FromNegative))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newUtf8String").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewUtf8String)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newLatin1String").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewLatin1String)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newUcs2String").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewUcs2String)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newStdString").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewStdString)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newRegExp").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewRegExp)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newStringObject").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewStringObject)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newNumberObject").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewNumberObject)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newBooleanObject").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewBooleanObject)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newExternal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewExternal)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newSignature").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewSignature)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newScript").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewScript)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newScript2").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewScript2)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("compileScript").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CompileScript)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("compileScript2").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CompileScript2)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newDate").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewDate)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newArray").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewArray)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newBoolean").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewBoolean)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("newBoolean2").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NewBoolean2)).ToLocalChecked()
  );
}

NODE_MODULE(news, Init)
