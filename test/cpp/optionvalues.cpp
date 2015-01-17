/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

NAN_METHOD(OptionValues) {
  NanScope();

  v8::Local<v8::Object> inobj = args[0].As<v8::Object>();
  v8::Local<v8::Object> outobj = NanNew<v8::Object>();

  bool boolt = NanBooleanOptionValue(inobj, NanNew("true"));
  bool boolf = NanBooleanOptionValue(inobj, NanNew("false"));
  bool booldt = NanBooleanOptionValue(inobj, NanNew("dt"), true);
  bool booldf = NanBooleanOptionValue(inobj, NanNew("df"), false);
  uint32_t uint32i = NanUInt32OptionValue(inobj, NanNew("i"), 0);
  uint32_t uint32f = NanUInt32OptionValue(inobj, NanNew("f"), 0);
  uint32_t uint32di = NanUInt32OptionValue(inobj, NanNew("di"), 111);

  outobj->Set(NanNew("true"), NanNew(boolt));
  outobj->Set(NanNew("false"), NanNew(boolf));
  outobj->Set(NanNew("dt"), NanNew(booldt));
  outobj->Set(NanNew("df"), NanNew(booldf));
  outobj->Set(NanNew("i"), NanNew(uint32i));
  outobj->Set(NanNew("f"), NanNew(uint32f));
  outobj->Set(NanNew("di"), NanNew(uint32di));

  NanReturnValue(outobj);
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanNew("o")
    , NanNew<v8::FunctionTemplate>(OptionValues)->GetFunction()
  );
}

NODE_MODULE(optionvalues, Init)
