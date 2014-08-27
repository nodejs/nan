/**********************************************************************************
* NAN - Native Abstractions for Node.js
*
* Copyright (c) 2014 NAN contributors
*
* MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
**********************************************************************************/

#include <nan.h>

class MyObject : public node::ObjectWrap {
public:
    static NAN_REGFUNC(Init);

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

NAN_REGFUNC(MyObject::Init) {
    NanScope();

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("MyObject"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(tpl, "call_emit", CallEmit);

    NanAssignPersistent<v8::Function>(constructor, tpl->GetFunction());
    exports->Set(NanNew<v8::String>("MyObject"), tpl->GetFunction());
}

NAN_METHOD(MyObject::New) {
    NanScope();

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
    NanScope();
    v8::Handle<v8::Value> argv[1] = {
        NanNew("event"), // event name
    };

    NanMakeCallback(args.This(), "emit", 1, argv);
    NanReturnUndefined();
}

NAN_REGFUNC(Init) {
    MyObject::Init(exports);
}

NAN_MODULE(makecallback, Init)
