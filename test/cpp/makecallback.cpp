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
    static NAN_METHOD(CallEmit);
    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> MyObject::constructor;

MyObject::MyObject() {
}

MyObject::~MyObject() {
}

void MyObject::Init(v8::Handle<v8::Object> exports) {
    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("MyObject"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(tpl, "call_emit", CallEmit);

    NanAssignPersistent<v8::Function>(constructor, tpl->GetFunction());
    exports->Set(NanNew<v8::String>("MyObject"), tpl->GetFunction());
}

NAN_METHOD(MyObject::New) {
    if (args.IsConstructCall()) {
        MyObject* obj = new MyObject();
        obj->Wrap(args.This());
        NanReturnValue(args.This());
    }
    else {
        v8::Local<v8::Function> cons = NanNew<v8::Function>(constructor);
        NanReturnValue(cons->NewInstance());
    }
}

NAN_METHOD(MyObject::CallEmit) {
    v8::Handle<v8::Value> argv[1] = {
        NanNew("event"), // event name
    };

    NanMakeCallback(args.This(), "emit", 1, argv);
    NanReturnUndefined();
}

void Init(v8::Handle<v8::Object> exports) {
    MyObject::Init(exports);
}

NODE_MODULE(makecallback, Init)
