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
	tpl->SetClassName(NanNew<v8::String>("MyObject"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NanSetPrototypeTemplate(tpl, "prototypeProp", NanNew<v8::String>("a prototype property"));
	// Instance
	NanSetInstanceTemplate(tpl, "instanceProp", NanNew<v8::String>("an instance property"));

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

void Init(v8::Handle<v8::Object> exports) {
	MyObject::Init(exports);
}

NODE_MODULE(settemplate, Init)
