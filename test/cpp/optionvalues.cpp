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

  bool boolt = NanBooleanOptionValue(
      inobj
    , NanNew("true").ToLocalChecked());
  bool boolf = NanBooleanOptionValue(
      inobj
    , NanNew("false").ToLocalChecked());
  bool booldt = NanBooleanOptionValue(
      inobj
    , NanNew("dt").ToLocalChecked()
    , true);
  bool booldf = NanBooleanOptionValue(
      inobj
    , NanNew("df").ToLocalChecked()
    , false);
  uint32_t uint32i = NanUInt32OptionValue(
      inobj
    , NanNew("i").ToLocalChecked()
    , 0);
  uint32_t uint32f = NanUInt32OptionValue(
      inobj
    , NanNew("f").ToLocalChecked()
    , 0);
  uint32_t uint32di = NanUInt32OptionValue(
      inobj
    , NanNew("di").ToLocalChecked()
    , 111);

  NanSet(outobj, NanNew("true").ToLocalChecked(), NanNew(boolt));
  NanSet(outobj, NanNew("false").ToLocalChecked(), NanNew(boolf));
  NanSet(outobj, NanNew("dt").ToLocalChecked(), NanNew(booldt));
  NanSet(outobj, NanNew("df").ToLocalChecked(), NanNew(booldf));
  NanSet(outobj, NanNew("i").ToLocalChecked(), NanNew(uint32i));
  NanSet(outobj, NanNew("f").ToLocalChecked(), NanNew(uint32f));
  NanSet(outobj, NanNew("di").ToLocalChecked(), NanNew(uint32di));

  NanReturnValue(outobj);
}

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew("o").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(OptionValues)->GetFunction()
  );
}

NODE_MODULE(optionvalues, Init)
