/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <cstring>

class NamedInterceptor : public NanObjectWrap {
  char buf[256];

 public:
  NamedInterceptor() { std::strncpy(this->buf, "foo", sizeof (this->buf)); }
  static void Init (v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> NewInstance ();
  static NAN_METHOD(New);

  static NAN_PROPERTY_GETTER(PropertyGetter);
  static NAN_PROPERTY_SETTER(PropertySetter);
  static NAN_PROPERTY_ENUMERATOR(PropertyEnumerator);
  static NAN_PROPERTY_DELETER(PropertyDeleter);
  static NAN_PROPERTY_QUERY(PropertyQuery);
};

static NanPersistent<v8::FunctionTemplate> namedinterceptors_constructor;

NAN_METHOD(CreateNew) {
  info.GetReturnValue().Set(NamedInterceptor::NewInstance());
}

void NamedInterceptor::Init(v8::Handle<v8::Object> target) {
  v8::Local<v8::FunctionTemplate> tpl =
    NanNew<v8::FunctionTemplate>(NamedInterceptor::New);
  namedinterceptors_constructor.Reset(tpl);
  tpl->SetClassName(NanNew("NamedInterceptor").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  v8::Local<v8::ObjectTemplate> inst = tpl->InstanceTemplate();

  NanSetNamedPropertyHandler(
      inst
    , NamedInterceptor::PropertyGetter
    , NamedInterceptor::PropertySetter
    , NamedInterceptor::PropertyQuery
    , NamedInterceptor::PropertyDeleter
    , NamedInterceptor::PropertyEnumerator);

  v8::Local<v8::Function> createnew =
    NanNew<v8::FunctionTemplate>(CreateNew)->GetFunction();
  NanSet(target, NanNew("create").ToLocalChecked(), createnew);
}

v8::Handle<v8::Value> NamedInterceptor::NewInstance () {
  NanEscapableScope scope;
  v8::Local<v8::FunctionTemplate> constructorHandle =
      NanNew(namedinterceptors_constructor);
  v8::Local<v8::Object> instance =
    constructorHandle->GetFunction()->NewInstance(0, NULL);
  return scope.Escape(instance);
}

NAN_METHOD(NamedInterceptor::New) {
  NamedInterceptor* interceptor = new NamedInterceptor();
  interceptor->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}


NAN_PROPERTY_GETTER(NamedInterceptor::PropertyGetter) {
  NamedInterceptor* interceptor =
    NanObjectWrap::Unwrap<NamedInterceptor>(info.This());
  if (!std::strcmp(*v8::String::Utf8Value(property), "prop")) {
    info.GetReturnValue().Set(NanNew(interceptor->buf).ToLocalChecked());
  } else {
    info.GetReturnValue().Set(NanNew("bar").ToLocalChecked());
  }
}

NAN_PROPERTY_SETTER(NamedInterceptor::PropertySetter) {
  NamedInterceptor* interceptor =
    NanObjectWrap::Unwrap<NamedInterceptor>(info.This());
  if (!std::strcmp(*v8::String::Utf8Value(property), "prop")) {
    std::strncpy(
        interceptor->buf
      , *v8::String::Utf8Value(value)
      , sizeof (interceptor->buf));
    info.GetReturnValue().Set(info.This());
  } else {
    info.GetReturnValue().Set(info.This());
  }
}

NAN_PROPERTY_ENUMERATOR(NamedInterceptor::PropertyEnumerator) {
  v8::Local<v8::Array> arr = NanNew<v8::Array>();
  NanSet(arr, 0, NanNew("value").ToLocalChecked());
  info.GetReturnValue().Set(arr);
}

NAN_PROPERTY_DELETER(NamedInterceptor::PropertyDeleter) {
  NamedInterceptor* interceptor =
    NanObjectWrap::Unwrap<NamedInterceptor>(info.This());
  std::strncpy(interceptor->buf, "goober", sizeof (interceptor->buf));
  info.GetReturnValue().Set(NanTrue());
}

NAN_PROPERTY_QUERY(NamedInterceptor::PropertyQuery) {
  if (!std::strcmp(*v8::String::Utf8Value(property), "thing")) {
    info.GetReturnValue().Set(NanNew<v8::Integer>(v8::DontEnum));
  }
}

NODE_MODULE(namedinterceptors, NamedInterceptor::Init)
