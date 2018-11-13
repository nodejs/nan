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

class IndexedInterceptor : public ObjectWrap {
  char buf[256];

 public:
  IndexedInterceptor() { std::strncpy(this->buf, "foo", sizeof (this->buf)); }
  static NAN_MODULE_INIT(Init);
  static v8::Local<v8::Value> NewInstance ();
  static NAN_METHOD(New);

  static NAN_INDEX_GETTER(PropertyGetter);
  static NAN_INDEX_SETTER(PropertySetter);
  static NAN_INDEX_ENUMERATOR(PropertyEnumerator);
  static NAN_INDEX_DELETER(PropertyDeleter);
  static NAN_INDEX_QUERY(PropertyQuery);
};

static Persistent<v8::FunctionTemplate> indexedinterceptors_constructor;

NAN_METHOD(CreateNew) {
  info.GetReturnValue().Set(IndexedInterceptor::NewInstance());
}

NAN_MODULE_INIT(IndexedInterceptor::Init) {
  v8::Local<v8::FunctionTemplate> tpl =
    Nan::New<v8::FunctionTemplate>(IndexedInterceptor::New);
  indexedinterceptors_constructor.Reset(tpl);
  tpl->SetClassName(Nan::New("IndexedInterceptor").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  v8::Local<v8::ObjectTemplate> inst = tpl->InstanceTemplate();

  SetIndexedPropertyHandler(
      inst
    , IndexedInterceptor::PropertyGetter
    , IndexedInterceptor::PropertySetter
    , IndexedInterceptor::PropertyQuery
    , IndexedInterceptor::PropertyDeleter
    , IndexedInterceptor::PropertyEnumerator);

  v8::Local<v8::Function> createnew =
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(CreateNew))
    .ToLocalChecked();
  Set(target, Nan::New("create").ToLocalChecked(), createnew);
}

v8::Local<v8::Value> IndexedInterceptor::NewInstance () {
  EscapableHandleScope scope;
  v8::Local<v8::FunctionTemplate> constructorHandle =
      Nan::New(indexedinterceptors_constructor);
  v8::Local<v8::Object> instance =
    Nan::NewInstance(Nan::GetFunction(constructorHandle).ToLocalChecked())
    .ToLocalChecked();
  return scope.Escape(instance);
}

NAN_METHOD(IndexedInterceptor::New) {
  IndexedInterceptor* interceptor = new IndexedInterceptor();
  interceptor->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}


NAN_INDEX_GETTER(IndexedInterceptor::PropertyGetter) {
  IndexedInterceptor* interceptor =
    ObjectWrap::Unwrap<IndexedInterceptor>(info.Holder());
  if (index == 0) {
    info.GetReturnValue().Set(Nan::New(interceptor->buf).ToLocalChecked());
  } else {
    info.GetReturnValue().Set(Nan::New("bar").ToLocalChecked());
  }
}

NAN_INDEX_SETTER(IndexedInterceptor::PropertySetter) {
  IndexedInterceptor* interceptor =
    ObjectWrap::Unwrap<IndexedInterceptor>(info.Holder());
  if (index == 0) {
    std::strncpy(
        interceptor->buf
      , *Nan::Utf8String(value)
      , sizeof (interceptor->buf));
    info.GetReturnValue().Set(info.This());
  } else {
    info.GetReturnValue().Set(info.This());
  }
}

NAN_INDEX_ENUMERATOR(IndexedInterceptor::PropertyEnumerator) {
  v8::Local<v8::Array> arr = Nan::New<v8::Array>();
  Set(arr, 0, Nan::New(42));
  info.GetReturnValue().Set(arr);
}

NAN_INDEX_DELETER(IndexedInterceptor::PropertyDeleter) {
  IndexedInterceptor* interceptor =
    ObjectWrap::Unwrap<IndexedInterceptor>(info.Holder());
  std::strncpy(interceptor->buf, "goober", sizeof (interceptor->buf));
  info.GetReturnValue().Set(True());
}

NAN_INDEX_QUERY(IndexedInterceptor::PropertyQuery) {
  if (index == 1) {
    info.GetReturnValue().Set(Nan::New<v8::Integer>(v8::DontEnum));
  }
  if (index == 42) {
    info.GetReturnValue().Set(Nan::New(0));
  }
}

NODE_MODULE(indexedinterceptors, IndexedInterceptor::Init)
