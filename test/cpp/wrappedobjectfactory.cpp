/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

// ref: https://nodejs.org/api/addons.html#addons_factory_of_wrapped_objects
class MyObject : public ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    SetPrototypeMethod(tpl, "getValue", GetValue);

    constructor.Reset(GetFunction(tpl).ToLocalChecked());
  }

  static NAN_METHOD(NewInstance) {
    v8::Local<v8::Function> cons = Nan::New(constructor);
    double value = info[0]->IsNumber() ? To<double>(info[0]).FromJust() : 0;
    const int argc = 1;
    v8::Local<v8::Value> argv[1] = {Nan::New(value)};
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }

 private:
  explicit MyObject(double value = 0) : value_(value) {}
  ~MyObject() {}

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      double value = info[0]->IsNumber() ? To<double>(info[0]).FromJust() : 0;
      MyObject * obj = new MyObject(value);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor);
      info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
  }

  static NAN_METHOD(GetValue) {
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.This());
    info.GetReturnValue().Set(obj->value_);
  }

  static Persistent<v8::Function> constructor;
  double value_;
};

Persistent<v8::Function> MyObject::constructor;

NAN_MODULE_INIT(Init) {
  MyObject::Init(target);
  Set(target
    , New<v8::String>("newFactoryObjectInstance").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(MyObject::NewInstance)).ToLocalChecked()
  );
}

NODE_MODULE(wrappedobjectfactory, Init)
