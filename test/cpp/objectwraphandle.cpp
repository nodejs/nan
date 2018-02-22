/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

class MyObject : public ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("MyObject").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    SetPrototypeMethod(tpl, "getHandle", GetHandle);
    SetPrototypeMethod(tpl, "getHandleConst", GetHandleConst);
    SetPrototypeMethod(tpl, "getValue", GetValue);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Set(target, Nan::New("MyObject").ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked());
  }

 private:
  explicit MyObject(double value = 0) : value_(value) {}
  ~MyObject() {}

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      double value =
          info[0]->IsUndefined() ? 0 : Nan::To<double>(info[0]).FromJust();
      MyObject *obj = new MyObject(value);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(
          Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
  }

  static NAN_METHOD(GetHandle) {
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
    info.GetReturnValue().Set(obj->handle());
  }

  static NAN_METHOD(GetHandleConst) {
    MyObject const *obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
    info.GetReturnValue().Set(obj->handle());
  }

  static NAN_METHOD(GetValue) {
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
    info.GetReturnValue().Set(obj->value_);
  }

  static inline Persistent<v8::Function> & constructor() {
    static Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  double value_;
};

NODE_MODULE(objectwraphandle, MyObject::Init)
