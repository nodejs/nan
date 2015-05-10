/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

class MyObject : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  MyObject();
  ~MyObject();

  static NAN_METHOD(New);
  static NanPersistent<v8::Function> constructor;
};

NanPersistent<v8::Function> MyObject::constructor;

MyObject::MyObject() {
}

MyObject::~MyObject() {
}

void MyObject::Init(v8::Handle<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
  tpl->SetClassName(NanNew<v8::String>("MyObject"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NanSetPrototypeTemplate(tpl, "prototypeProp", NanNew("a prototype property"));
  // Instance
  NanSetInstanceTemplate(tpl, "instanceProp", NanNew("an instance property"));
  // PropertyAttributes
  NanSetInstanceTemplate(
      tpl, NanNew("none"), NanNew("none"), v8::None);
  NanSetInstanceTemplate(
      tpl, NanNew("readOnly"), NanNew("readOnly"), v8::ReadOnly);
  NanSetInstanceTemplate(
      tpl, NanNew("dontEnum"), NanNew("dontEnum"), v8::DontEnum);
  NanSetInstanceTemplate(
      tpl, NanNew("dontDelete"), NanNew("dontDelete"), v8::DontDelete);

  constructor.Reset(tpl->GetFunction());
  exports->Set(NanNew<v8::String>("MyObject"), tpl->GetFunction());
}

NAN_METHOD(MyObject::New) {
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
