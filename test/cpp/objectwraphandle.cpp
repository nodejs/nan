/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

class MyObject : public NanObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew("MyObject"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(tpl, "getHandle", GetHandle);

    constructor.Reset(tpl->GetFunction());
    exports->Set(NanNew("MyObject"), tpl->GetFunction());
  }

 private:
  explicit MyObject(double value = 0) : value_(value) {}
  ~MyObject() {}

  static NAN_METHOD(New) {
    if (args.IsConstructCall()) {
      double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
      MyObject *obj = new MyObject(value);
      obj->Wrap(args.This());
      NanReturnThis();
    } else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {args[0]};
      v8::Local<v8::Function> cons = NanNew(constructor);
      NanReturnValue(cons->NewInstance(argc, argv));
    }
  }

  static NAN_METHOD(GetHandle) {
    MyObject* obj = NanObjectWrap::Unwrap<MyObject>(args.This());
    NanReturnValue(obj->handle());
  }

  static NanPersistent<v8::Function> constructor;
  double value_;
};

NanPersistent<v8::Function> MyObject::constructor;

void Init(v8::Handle<v8::Object> exports) {
  MyObject::Init(exports);
}

NODE_MODULE(objectwraphandle, Init)
