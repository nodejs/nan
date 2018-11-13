/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <cstring>

using namespace Nan;  // NOLINT(build/namespaces)

class NamedInterceptor : public ObjectWrap {
  char buf[256];

 public:
  NamedInterceptor() { std::strncpy(this->buf, "foo", sizeof (this->buf)); }
  static NAN_MODULE_INIT(Init);
  static v8::Local<v8::Value> NewInstance ();
  static NAN_METHOD(New);

  static NAN_PROPERTY_GETTER(PropertyGetter);
  static NAN_PROPERTY_SETTER(PropertySetter);
  static NAN_PROPERTY_ENUMERATOR(PropertyEnumerator);
  static NAN_PROPERTY_DELETER(PropertyDeleter);
  static NAN_PROPERTY_QUERY(PropertyQuery);
};

static Persistent<v8::FunctionTemplate> namedinterceptors_constructor;

NAN_METHOD(CreateNew) {
  info.GetReturnValue().Set(NamedInterceptor::NewInstance());
}

NAN_MODULE_INIT(NamedInterceptor::Init) {
  v8::Local<v8::FunctionTemplate> tpl =
    Nan::New<v8::FunctionTemplate>(NamedInterceptor::New);
  namedinterceptors_constructor.Reset(tpl);
  tpl->SetClassName(Nan::New("NamedInterceptor").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  v8::Local<v8::ObjectTemplate> inst = tpl->InstanceTemplate();

  SetNamedPropertyHandler(
      inst
    , NamedInterceptor::PropertyGetter
    , NamedInterceptor::PropertySetter
    , NamedInterceptor::PropertyQuery
    , NamedInterceptor::PropertyDeleter
    , NamedInterceptor::PropertyEnumerator);

  v8::Local<v8::Function> createnew =
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(CreateNew))
    .ToLocalChecked();
  Set(target, Nan::New("create").ToLocalChecked(), createnew);
}

v8::Local<v8::Value> NamedInterceptor::NewInstance () {
  EscapableHandleScope scope;
  v8::Local<v8::FunctionTemplate> constructorHandle =
      Nan::New(namedinterceptors_constructor);
  v8::Local<v8::Object> instance =
    Nan::NewInstance(GetFunction(constructorHandle).ToLocalChecked())
    .ToLocalChecked();
  return scope.Escape(instance);
}

NAN_METHOD(NamedInterceptor::New) {
  NamedInterceptor* interceptor = new NamedInterceptor();
  interceptor->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}


NAN_PROPERTY_GETTER(NamedInterceptor::PropertyGetter) {
  NamedInterceptor* interceptor =
    ObjectWrap::Unwrap<NamedInterceptor>(info.Holder());
  if (!std::strcmp(*Nan::Utf8String(property), "prop")) {
    info.GetReturnValue().Set(Nan::New(interceptor->buf).ToLocalChecked());
  } else {
    info.GetReturnValue().Set(Nan::New("bar").ToLocalChecked());
  }
}

NAN_PROPERTY_SETTER(NamedInterceptor::PropertySetter) {
  NamedInterceptor* interceptor =
    ObjectWrap::Unwrap<NamedInterceptor>(info.Holder());
  if (!std::strcmp(*Nan::Utf8String(property), "prop")) {
    std::strncpy(
        interceptor->buf
      , *Nan::Utf8String(value)
      , sizeof (interceptor->buf));
    info.GetReturnValue().Set(info.This());
  } else {
    info.GetReturnValue().Set(info.This());
  }
}

NAN_PROPERTY_ENUMERATOR(NamedInterceptor::PropertyEnumerator) {
  v8::Local<v8::Array> arr = Nan::New<v8::Array>();
  Set(arr, 0, Nan::New("value").ToLocalChecked());
  info.GetReturnValue().Set(arr);
}

NAN_PROPERTY_DELETER(NamedInterceptor::PropertyDeleter) {
  NamedInterceptor* interceptor =
    ObjectWrap::Unwrap<NamedInterceptor>(info.Holder());
  std::strncpy(interceptor->buf, "goober", sizeof (interceptor->buf));
  info.GetReturnValue().Set(True());
}

NAN_PROPERTY_QUERY(NamedInterceptor::PropertyQuery) {
  Nan::Utf8String s(property);
  if (!std::strcmp(*s, "thing")) {
    return info.GetReturnValue().Set(Nan::New<v8::Integer>(v8::DontEnum));
  }
  if (!std::strcmp(*s, "value")) {
    return info.GetReturnValue().Set(Nan::New(0));
  }
}

NODE_MODULE(namedinterceptors, NamedInterceptor::Init)
