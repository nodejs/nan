/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2017 NAN contributors
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
  static NAN_METHOD(CallEmit);
  static Persistent<v8::Function> constructor;
};

Persistent<v8::Function> MyObject::constructor;

MyObject::MyObject() {
}

MyObject::~MyObject() {
}

NAN_MODULE_INIT(MyObject::Init) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New<v8::String>("MyObject").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  SetPrototypeMethod(tpl, "call_emit", CallEmit);

  constructor.Reset(tpl->GetFunction());
  Set(target, Nan::New("MyObject").ToLocalChecked(), tpl->GetFunction());
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

NAN_METHOD(MyObject::CallEmit) {
  v8::Local<v8::Value> argv[1] = {
    Nan::New("event").ToLocalChecked(),  // event name
  };

  MakeCallback(info.This(), "emit", 1, argv);
  info.GetReturnValue().SetUndefined();
}

NODE_MODULE(makecallback, MyObject::Init)
