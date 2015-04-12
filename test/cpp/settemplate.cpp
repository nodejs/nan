/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

class MyObject : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  MyObject();
  ~MyObject();

  static NAN_METHOD(New);
  static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> MyObject::constructor;

MyObject::MyObject() {
}

MyObject::~MyObject() {
}

void MyObject::Init(v8::Handle<v8::Object> exports) {
  NanScope();

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
  tpl->SetClassName(NanNew<v8::String>("MyObject").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NanSetPrototypeTemplate(
    tpl
  , "prototypeProp"
  , NanNew<v8::String>("a prototype property").ToLocalChecked());

  // Instance
  NanSetInstanceTemplate(
    tpl
  , "instanceProp"
  , NanNew<v8::String>("an instance property").ToLocalChecked());

  // PropertyAttributes
  NanSetInstanceTemplate(
    tpl
  , NanNew<v8::String>("none").ToLocalChecked()
  , NanNew<v8::String>("none").ToLocalChecked()
  , v8::None);
  NanSetInstanceTemplate(
    tpl
  , NanNew<v8::String>("readOnly").ToLocalChecked()
  , NanNew<v8::String>("readOnly").ToLocalChecked()
  , v8::ReadOnly);
  NanSetInstanceTemplate(
    tpl
  , NanNew<v8::String>("dontEnum").ToLocalChecked()
  , NanNew<v8::String>("dontEnum").ToLocalChecked()
  , v8::DontEnum);
  NanSetInstanceTemplate(
    tpl
  , NanNew<v8::String>("dontDelete").ToLocalChecked()
  , NanNew<v8::String>("dontDelete").ToLocalChecked()
  , v8::DontDelete);

  NanAssignPersistent<v8::Function>(constructor, tpl->GetFunction());
  NanSet(exports
  , NanNew<v8::String>("MyObject").ToLocalChecked()
  , tpl->GetFunction());
}

NAN_METHOD(MyObject::New) {
  NanScope();

  if (args.IsConstructCall()) {
    MyObject* obj = new MyObject();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    v8::Local<v8::Function> cons = NanNew<v8::Function>(constructor);
    NanReturnValue(cons->NewInstance());
  }
}

void Init(v8::Handle<v8::Object> exports) {
  MyObject::Init(exports);
}

NODE_MODULE(settemplate, Init)
