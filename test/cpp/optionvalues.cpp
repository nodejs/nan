#include <node.h>
#include "../../nan.h"

NAN_METHOD(OptionValues) {
  NanScope();

  v8::Local<v8::Object> inobj = args[0].As<v8::Object>();
  v8::Local<v8::Object> outobj = v8::Object::New();

  bool boolt = NanBooleanOptionValue(inobj, NanSymbol("true"));
  bool boolf = NanBooleanOptionValue(inobj, NanSymbol("false"));
  bool booldt = NanBooleanOptionValue(inobj, NanSymbol("dt"), true);
  bool booldf = NanBooleanOptionValue(inobj, NanSymbol("df"), false);
  uint32_t uint32i = NanUInt32OptionValue(inobj, NanSymbol("i"), 0);
  uint32_t uint32f = NanUInt32OptionValue(inobj, NanSymbol("f"), 0);
  uint32_t uint32di = NanUInt32OptionValue(inobj, NanSymbol("di"), 111);

  outobj->Set(NanSymbol("true"), v8::Boolean::New(boolt));
  outobj->Set(NanSymbol("false"), v8::Boolean::New(boolf));
  outobj->Set(NanSymbol("dt"), v8::Boolean::New(booldt));
  outobj->Set(NanSymbol("df"), v8::Boolean::New(booldf));
  outobj->Set(NanSymbol("i"), v8::Number::New(uint32i));
  outobj->Set(NanSymbol("f"), v8::Number::New(uint32f));
  outobj->Set(NanSymbol("di"), v8::Number::New(uint32di));

  NanReturnValue(outobj);
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("o")
    , v8::FunctionTemplate::New(OptionValues)->GetFunction()
  );
}

NODE_MODULE(optionvalues, Init)