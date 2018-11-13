/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

class MyObject : public node::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);

 private:
  MyObject();
  ~MyObject();

  static NAN_METHOD(New);
  static Persistent<v8::Function> constructor;
};

Persistent<v8::Function> MyObject::constructor;

MyObject::MyObject() {
}

MyObject::~MyObject() {
}

void Foo(FunctionCallbackInfo<v8::Value> const&) {}

NAN_MODULE_INIT(MyObject::Init) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New<v8::String>("MyObject").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  SetPrototypeTemplate(
    tpl
  , "prototypeProp"
  , Nan::New<v8::String>("a prototype property").ToLocalChecked());

  // Instance
  SetInstanceTemplate(
    tpl
  , "instanceProp"
  , Nan::New<v8::String>("an instance property").ToLocalChecked());

  // PropertyAttributes
  SetInstanceTemplate(
    tpl
  , Nan::New<v8::String>("none").ToLocalChecked()
  , Nan::New<v8::String>("none").ToLocalChecked()
  , v8::None);
  SetInstanceTemplate(
    tpl
  , Nan::New<v8::String>("readOnly").ToLocalChecked()
  , Nan::New<v8::String>("readOnly").ToLocalChecked()
  , v8::ReadOnly);
  SetInstanceTemplate(
    tpl
  , Nan::New<v8::String>("dontEnum").ToLocalChecked()
  , Nan::New<v8::String>("dontEnum").ToLocalChecked()
  , v8::DontEnum);
  SetInstanceTemplate(
    tpl
  , Nan::New<v8::String>("dontDelete").ToLocalChecked()
  , Nan::New<v8::String>("dontDelete").ToLocalChecked()
  , v8::DontDelete);

  v8::Local<v8::Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Set(target
  , Nan::New<v8::String>("MyObject").ToLocalChecked()
  , function);


  //=== SetMethod ==============================================================

  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  SetMethod(obj, "foo", Foo);

  // https://github.com/nodejs/nan/issues/564
  v8::Local<v8::Function> func = Nan::New<v8::Function>(Foo);
  SetMethod(func, "foo", Foo);

  v8::Local<v8::FunctionTemplate> t = Nan::New<v8::FunctionTemplate>(Foo);
  SetMethod(t, "foo", Foo);
}

NAN_METHOD(MyObject::New) {
  if (info.IsConstructCall()) {
    MyObject* obj = new MyObject();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(Nan::NewInstance(cons).ToLocalChecked());
  }
}

NODE_MODULE(settemplate, MyObject::Init)
