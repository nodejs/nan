/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <node.h>
#include <nan.h>

NAN_METHOD(OptionValues) {
  NanScope();

  v8::Local<v8::Object> inobj = args[0].As<v8::Object>();
  v8::Local<v8::Object> outobj = NanNew<v8::Object>();

  bool boolt = NanBooleanOptionValue(inobj, NanSymbol("true"));
  bool boolf = NanBooleanOptionValue(inobj, NanSymbol("false"));
  bool booldt = NanBooleanOptionValue(inobj, NanSymbol("dt"), true);
  bool booldf = NanBooleanOptionValue(inobj, NanSymbol("df"), false);
  uint32_t uint32i = NanUInt32OptionValue(inobj, NanSymbol("i"), 0);
  uint32_t uint32f = NanUInt32OptionValue(inobj, NanSymbol("f"), 0);
  uint32_t uint32di = NanUInt32OptionValue(inobj, NanSymbol("di"), 111);

  outobj->Set(NanSymbol("true"), NanNew<v8::Boolean>(boolt));
  outobj->Set(NanSymbol("false"), NanNew<v8::Boolean>(boolf));
  outobj->Set(NanSymbol("dt"), NanNew<v8::Boolean>(booldt));
  outobj->Set(NanSymbol("df"), NanNew<v8::Boolean>(booldf));
  outobj->Set(NanSymbol("i"), NanNew<v8::Number>(uint32i));
  outobj->Set(NanSymbol("f"), NanNew<v8::Number>(uint32f));
  outobj->Set(NanSymbol("di"), NanNew<v8::Number>(uint32di));

  NanReturnValue(outobj);
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("o")
    , NanNew<v8::FunctionTemplate>(OptionValues)->GetFunction()
  );
}

NODE_MODULE(optionvalues, Init)
