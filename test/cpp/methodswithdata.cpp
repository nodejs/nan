/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2019 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(TestWithData) {
  info.GetReturnValue().Set(false);
  {
    std::string datavalue = *Nan::Utf8String(info.Data());
    if (datavalue != "test-value")return;
  }
  info.GetReturnValue().Set(true);
}

class SetterGetter : public ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);
  static v8::Local<v8::Value> NewInstance ();
  static NAN_METHOD(New);
  static NAN_METHOD(Log);
  static NAN_GETTER(GetProp1);
  static NAN_GETTER(GetProp2);
  static NAN_SETTER(SetProp2);

  SetterGetter();

  char log[1024];
  char prop1[256];
  char prop2[256];
};

static Persistent<v8::FunctionTemplate> settergetter_constructor;

SetterGetter::SetterGetter() {
  log[0] = '\0';
  strncpy(prop1, "this is property 1", sizeof (prop1) - 1);
  prop1[sizeof (prop1) - 1] = '\0';
  prop2[0] = '\0';
}

v8::Local<v8::Value> SetterGetter::NewInstance () {
  EscapableHandleScope scope;
  v8::Local<v8::FunctionTemplate> constructorHandle =
      Nan::New(settergetter_constructor);
  v8::Local<v8::Object> instance =
    Nan::NewInstance(
        Nan::GetFunction(constructorHandle).ToLocalChecked()).ToLocalChecked();
  return scope.Escape(instance);
}

NAN_METHOD(SetterGetter::New) {
  std::string datavalue = *Nan::Utf8String(info.Data());
  assert(datavalue == "new-data");

  SetterGetter* settergetter = new SetterGetter();
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , "New()\n"
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));
  settergetter->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

NAN_GETTER(SetterGetter::GetProp1) {
  std::string datavalue = *Nan::Utf8String(info.Data());
  assert(datavalue == "prop1-data");
  SetterGetter* settergetter =
    ObjectWrap::Unwrap<SetterGetter>(info.Holder());
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , "Prop1:GETTER("
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , settergetter->prop1
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , ")\n"
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));

  info.GetReturnValue().Set(Nan::New(settergetter->prop1).ToLocalChecked());
}

NAN_GETTER(SetterGetter::GetProp2) {
  std::string datavalue = *Nan::Utf8String(info.Data());
  assert(datavalue == "prop2-data");

  SetterGetter* settergetter =
    ObjectWrap::Unwrap<SetterGetter>(info.Holder());
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , "Prop2:GETTER("
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , settergetter->prop2
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , ")\n"
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));

  info.GetReturnValue().Set(Nan::New(settergetter->prop2).ToLocalChecked());
}

NAN_SETTER(SetterGetter::SetProp2) {
  std::string datavalue = *Nan::Utf8String(info.Data());
  assert(datavalue == "prop2-data");

  SetterGetter* settergetter =
      ObjectWrap::Unwrap<SetterGetter>(info.Holder());
  strncpy(
      settergetter->prop2
    , *Utf8String(value)
    , sizeof (settergetter->prop2));
  settergetter->prop2[sizeof (settergetter->prop2) - 1] = '\0';
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , "Prop2:SETTER("
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , settergetter->prop2
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , ")\n"
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));
}

NAN_METHOD(SetterGetter::Log) {
  SetterGetter* settergetter =
    ObjectWrap::Unwrap<SetterGetter>(info.Holder());

  info.GetReturnValue().Set(Nan::New(settergetter->log).ToLocalChecked());
}

NAN_METHOD(CreateNew) {
  std::string datavalue = *Nan::Utf8String(info.Data());
  assert(datavalue == "create-data");

  info.GetReturnValue().Set(SetterGetter::NewInstance());
}

NAN_MODULE_INIT(Init) {
  SetMethod(target, "testWithData", TestWithData, New("test-value").ToLocalChecked());

  v8::Local<v8::FunctionTemplate> tpl =
    Nan::New<v8::FunctionTemplate>(SetterGetter::New, New("new-data").ToLocalChecked());
  settergetter_constructor.Reset(tpl);
  tpl->SetClassName(Nan::New<v8::String>("SetterGetter").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  SetPrototypeMethod(tpl, "log", SetterGetter::Log, Nan::New("log-data").ToLocalChecked());
  v8::Local<v8::ObjectTemplate> itpl = tpl->InstanceTemplate();
  SetAccessor(
      itpl
    , Nan::New("prop1").ToLocalChecked()
    , SetterGetter::GetProp1
    , 0
    , Nan::New("prop1-data").ToLocalChecked());
  SetAccessor(
      itpl
    , Nan::New<v8::String>("prop2").ToLocalChecked()
    , SetterGetter::GetProp2
    , SetterGetter::SetProp2
    , Nan::New("prop2-data").ToLocalChecked()
  );

  v8::Local<v8::Function> createnew =
    Nan::GetFunction(
        Nan::New<v8::FunctionTemplate>(CreateNew, Nan::New("create-data").ToLocalChecked())).ToLocalChecked();
  Set(target, Nan::New<v8::String>("create").ToLocalChecked(), createnew);
}

NODE_MODULE(methodswithdata, Init)
