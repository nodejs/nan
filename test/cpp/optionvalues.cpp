/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

NAN_METHOD(OptionValues) {
  NanScope();

  v8::Local<v8::Object> inobj = args[0].As<v8::Object>();
  v8::Local<v8::Object> outobj = NanNew<v8::Object>();

  bool boolt = NanBooleanOptionValue(inobj, NanNew<v8::String>("true"));
  bool boolf = NanBooleanOptionValue(inobj, NanNew<v8::String>("false"));
  bool booldt = NanBooleanOptionValue(inobj, NanNew<v8::String>("dt"), true);
  bool booldf = NanBooleanOptionValue(inobj, NanNew<v8::String>("df"), false);
  uint32_t uint32i = NanUInt32OptionValue(inobj, NanNew<v8::String>("i"), 0);
  uint32_t uint32f = NanUInt32OptionValue(inobj, NanNew<v8::String>("f"), 0);
  uint32_t uint32di = NanUInt32OptionValue(inobj, NanNew<v8::String>("di"), 111);

  outobj->Set(NanNew<v8::String>("true"), NanNew<v8::Boolean>(boolt));
  outobj->Set(NanNew<v8::String>("false"), NanNew<v8::Boolean>(boolf));
  outobj->Set(NanNew<v8::String>("dt"), NanNew<v8::Boolean>(booldt));
  outobj->Set(NanNew<v8::String>("df"), NanNew<v8::Boolean>(booldf));
  outobj->Set(NanNew<v8::String>("i"), NanNew<v8::Number>(uint32i));
  outobj->Set(NanNew<v8::String>("f"), NanNew<v8::Number>(uint32f));
  outobj->Set(NanNew<v8::String>("di"), NanNew<v8::Number>(uint32di));

  NanReturnValue(outobj);
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanNew<v8::String>("o")
    , NanNew<v8::FunctionTemplate>(OptionValues)->GetFunction()
  );
}

NODE_MODULE(optionvalues, Init)
