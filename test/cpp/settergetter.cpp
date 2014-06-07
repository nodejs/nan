/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>
#include <cstring>

class SetterGetter : public node::ObjectWrap {
 public:
  static void Init (v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> NewInstance ();
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

static v8::Persistent<v8::FunctionTemplate> settergetter_constructor;

NAN_METHOD(CreateNew) {
  NanScope();
  NanReturnValue(SetterGetter::NewInstance());
}

SetterGetter::SetterGetter() {
  log[0] = '\0';
  strncpy(prop1, "this is property 1", sizeof (prop1) - 1);
  prop1[sizeof (prop1) - 1] = '\0';
  prop2[0] = '\0';
}

void SetterGetter::Init(v8::Handle<v8::Object> target) {
  v8::Local<v8::FunctionTemplate> tpl =
    NanNew<v8::FunctionTemplate>(SetterGetter::New);
  NanAssignPersistent(settergetter_constructor, tpl);
  tpl->SetClassName(NanNew<v8::String>("SetterGetter"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(tpl, "log", SetterGetter::Log);
  v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
  proto->SetAccessor(NanNew<v8::String>("prop1"), SetterGetter::GetProp1);
  proto->SetAccessor(
    NanNew<v8::String>("prop2")
  , SetterGetter::GetProp2
  , SetterGetter::SetProp2
  );

  v8::Local<v8::Function> createnew =
    NanNew<v8::FunctionTemplate>(CreateNew)->GetFunction();
  target->Set(NanNew<v8::String>("create"), createnew);
}

v8::Handle<v8::Value> SetterGetter::NewInstance () {
  v8::Local<v8::FunctionTemplate> constructorHandle =
      NanNew(settergetter_constructor);
  v8::Local<v8::Object> instance =
    constructorHandle->GetFunction()->NewInstance(0, NULL);
  return instance;
}

NAN_METHOD(SetterGetter::New) {
  NanScope();

  SetterGetter* settergetter = new SetterGetter();
  assert(strlen(settergetter->log) < sizeof (settergetter->log));
  strncat(
      settergetter->log
    , "New()\n"
    , sizeof (settergetter->log) - 1 - strlen(settergetter->log));
  settergetter->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_GETTER(SetterGetter::GetProp1) {
  NanScope();

  SetterGetter* settergetter =
    node::ObjectWrap::Unwrap<SetterGetter>(args.This());
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

  NanReturnValue(NanNew(settergetter->prop1));
}

NAN_GETTER(SetterGetter::GetProp2) {
  NanScope();

  SetterGetter* settergetter =
    node::ObjectWrap::Unwrap<SetterGetter>(args.This());
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

  NanReturnValue(NanNew(settergetter->prop2));
}

NAN_SETTER(SetterGetter::SetProp2) {
  NanScope();

  SetterGetter* settergetter =
    node::ObjectWrap::Unwrap<SetterGetter>(args.This());
    settergetter->prop2.assign(NanUtf8String(value)());
  settergetter->log.append("Prop2:SETTER(");
  settergetter->log.append(settergetter->prop2);
  settergetter->log.append(")\n");
}

NAN_METHOD(SetterGetter::Log) {
  NanScope();

  SetterGetter* settergetter =
    node::ObjectWrap::Unwrap<SetterGetter>(args.This());

  NanReturnValue(NanNew(settergetter->log));
}

NODE_MODULE(settergetter, SetterGetter::Init)
