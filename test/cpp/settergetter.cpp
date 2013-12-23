#include <node.h>
#include "nan.h"
#include <string>

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

  std::string log;
  std::string prop1;
  std::string prop2;
};

static v8::Persistent<v8::FunctionTemplate> settergetter_constructor;

NAN_METHOD(CreateNew) {
  NanScope();
  NanReturnValue(SetterGetter::NewInstance());
}

SetterGetter::SetterGetter() {
  prop1.assign("this is property 1");
}

void SetterGetter::Init(v8::Handle<v8::Object> target) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(SetterGetter::New)
;
  NanAssignPersistent(v8::FunctionTemplate, settergetter_constructor, tpl);
  tpl->SetClassName(NanSymbol("SetterGetter"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(tpl, "log", SetterGetter::Log);
  v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();
  proto->SetAccessor(NanSymbol("prop1"), SetterGetter::GetProp1);
  proto->SetAccessor(NanSymbol("prop2"), SetterGetter::GetProp2, SetterGetter::SetProp2);

  v8::Local<v8::Function> createnew =
    v8::FunctionTemplate::New(CreateNew)->GetFunction();
  target->Set(NanSymbol("create"), createnew);
}

v8::Handle<v8::Value> SetterGetter::NewInstance () {
  NanScope();
  v8::Local<v8::FunctionTemplate> constructorHandle =
      NanPersistentToLocal(settergetter_constructor);
  v8::Local<v8::Object> instance =
    constructorHandle->GetFunction()->NewInstance(0, NULL);
  return instance;
}

NAN_METHOD(SetterGetter::New) {
  NanScope();

  SetterGetter* settergetter = new SetterGetter();
  settergetter->log.append("New()\n");
  settergetter->Wrap(args.This());

  NanReturnValue(args.This());
}

NAN_GETTER(SetterGetter::GetProp1) {
  NanScope();

  SetterGetter* settergetter =
    node::ObjectWrap::Unwrap<SetterGetter>(args.This());
  settergetter->log.append("Prop1:GETTER(");
  settergetter->log.append(settergetter->prop1);
  settergetter->log.append(")\n");

  NanReturnValue(v8::String::New(settergetter->prop1.c_str()));
}

NAN_GETTER(SetterGetter::GetProp2) {
  NanScope();

  SetterGetter* settergetter =
    node::ObjectWrap::Unwrap<SetterGetter>(args.This());
  settergetter->log.append("Prop2:GETTER(");
  settergetter->log.append(settergetter->prop2);
  settergetter->log.append(")\n");

  NanReturnValue(v8::String::New(settergetter->prop2.c_str()));
}

NAN_SETTER(SetterGetter::SetProp2) {
  NanScope();

  SetterGetter* settergetter =
    node::ObjectWrap::Unwrap<SetterGetter>(args.This());
  settergetter->prop2.assign(NanCString(value, NULL));
  settergetter->log.append("Prop2:SETTER(");
  settergetter->log.append(settergetter->prop2);
  settergetter->log.append(")\n");
}

NAN_METHOD(SetterGetter::Log) {
  NanScope();

  SetterGetter* settergetter =
    node::ObjectWrap::Unwrap<SetterGetter>(args.This());

  NanReturnValue(v8::String::New(settergetter->log.c_str()));
}

NODE_MODULE(settergetter, SetterGetter::Init)
