/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#if defined(_MSC_VER)
# pragma warning( disable : 4530 )
# include <string>
# pragma warning( default : 4530 )
#else
# include <string>
#endif

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

NAN_METHOD(NewUnsignedInteger) {
  NanScope();
  NanReturnValue(NanNew<v8::Integer>(0xFFFFFFFFu));
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
  NanReturnValue(NanNew<v8::String>(s).ToLocalChecked());
}

NAN_METHOD(NewLatin1String) {
  NanScope();
  const uint8_t s[] = "str\xefng";
  NanReturnValue(NanNew<v8::String>(s).ToLocalChecked());
}

NAN_METHOD(NewUcs2String) {
  NanScope();
  const uint16_t s[] = {'s', 't', 'r', 0xef, 'n', 'g', '\0'};
  NanReturnValue(NanNew(s).ToLocalChecked());
}

NAN_METHOD(NewStdString) {
  NanScope();
  const std::string s = "strïng";
  NanReturnValue(NanNew<v8::String>(s).ToLocalChecked());
}

NAN_METHOD(NewRegExp) {
  NanScope();
  NanReturnValue(NanNew<v8::RegExp>(
      NanNew("foo").ToLocalChecked(), v8::RegExp::kNone).ToLocalChecked());
}

NAN_METHOD(NewStringObject) {
  NanScope();
  NanReturnValue(NanNew<v8::StringObject>(NanNew("foo").ToLocalChecked()));
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
  NanReturnValue(NanNew<v8::String>("passed").ToLocalChecked());
}

NAN_METHOD(NewSignature) {
  NanScope();
  v8::Local<v8::FunctionTemplate> tmpl =
      NanNew<v8::FunctionTemplate>(NewSignature);
  v8::Local<v8::Signature> sig = NanNew<v8::Signature>(tmpl);
  tmpl = NanNew<v8::FunctionTemplate>(
      NewSignature, v8::Handle<v8::Value>(), sig);
  NanReturnValue(NanNew<v8::String>("string").ToLocalChecked());
}

NAN_METHOD(NewScript) {
  NanScope();
  v8::Local<NanUnboundScript> script =
      NanNew<NanUnboundScript>(NanNew("2+4").ToLocalChecked()).ToLocalChecked();
  NanReturnValue(
      NanToInt32(NanRunScript(script).ToLocalChecked()).ToLocalChecked());
}

NAN_METHOD(NewScript2) {
  NanScope();
  v8::ScriptOrigin origin(NanNew<v8::String>("x").ToLocalChecked());
  v8::Local<NanUnboundScript> script =
      NanNew<NanUnboundScript>(
          NanNew("2+4").ToLocalChecked()
        , origin).ToLocalChecked();
  NanReturnValue(
      NanToInt32(NanRunScript(script).ToLocalChecked()).ToLocalChecked());
}

NAN_METHOD(CompileScript) {
  NanScope();
  v8::Local<NanBoundScript> script =
      NanCompileScript(NanNew("2+4").ToLocalChecked()).ToLocalChecked();
  NanReturnValue(
      NanToInt32(NanRunScript(script).ToLocalChecked()).ToLocalChecked());
}

NAN_METHOD(CompileScript2) {
  NanScope();
  v8::ScriptOrigin origin(NanNew<v8::String>("x").ToLocalChecked());
  v8::Local<NanBoundScript> script =
      NanCompileScript(NanNew("2+4").ToLocalChecked(), origin).ToLocalChecked();
  NanReturnValue(
      NanToInt32(NanRunScript(script).ToLocalChecked()).ToLocalChecked());
}

NAN_METHOD(NewDate) {
  NanScope();
  NanReturnValue(NanNew<v8::Date>(1337).ToLocalChecked());
}

NAN_METHOD(NewArray) {
  NanScope();
  NanReturnValue(NanNew<v8::Array>());
}

NAN_METHOD(NewBoolean) {
  NanScope();
  NanReturnValue(NanNew<v8::Boolean>(true));
}

// #212
NAN_METHOD(NewBoolean2) {
  NanScope();
  #if defined(_MSC_VER)
  # pragma warning( disable : 4800 )
  #endif
  NanReturnValue(NanNew<v8::Boolean>(1));
  #if defined(_MSC_VER)
  # pragma warning( default : 4800 )
  #endif
}

void Init(v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("newNumber").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewNumber)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newNegativeInteger").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewNegativeInteger)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newPositiveInteger").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewPositiveInteger)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newUnsignedInteger").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewUnsignedInteger)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newInt32FromPositive").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewInt32FromPositive)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newInt32FromNegative").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewInt32FromNegative)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newUint32FromPositive").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewUint32FromPositive)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newUint32FromNegative").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewUint32FromNegative)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newUtf8String").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewUtf8String)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newLatin1String").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewLatin1String)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newUcs2String").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewUcs2String)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newStdString").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewStdString)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newRegExp").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewRegExp)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newStringObject").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewStringObject)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newNumberObject").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewNumberObject)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newBooleanObject").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewBooleanObject)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newExternal").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewExternal)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newSignature").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewSignature)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newScript").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewScript)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newScript2").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewScript2)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("compileScript").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(CompileScript)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("compileScript2").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(CompileScript2)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newDate").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewDate)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newArray").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewArray)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newBoolean").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewBoolean)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("newBoolean2").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(NewBoolean2)->GetFunction()
  );
}

NODE_MODULE(news, Init)
