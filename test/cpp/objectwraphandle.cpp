/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

class MyObject : public NanObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew("MyObject").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    NanSetPrototypeMethod(tpl, "getHandle", GetHandle);

    constructor.Reset(tpl->GetFunction());
    NanSet(exports, NanNew("MyObject").ToLocalChecked(), tpl->GetFunction());
  }

 private:
  explicit MyObject(double value = 0) : value_(value) {}
  ~MyObject() {}

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      double value = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
      MyObject *obj = new MyObject(value);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = NanNew(constructor);
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
  }

  static NAN_METHOD(GetHandle) {
    MyObject* obj = NanObjectWrap::Unwrap<MyObject>(info.This());
    info.GetReturnValue().Set(obj->handle());
  }

  static NanPersistent<v8::Function> constructor;
  double value_;
};

NanPersistent<v8::Function> MyObject::constructor;

void Init(v8::Handle<v8::Object> exports) {
  MyObject::Init(exports);
}

NODE_MODULE(objectwraphandle, Init)
