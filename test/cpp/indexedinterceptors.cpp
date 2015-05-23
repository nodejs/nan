/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <cstring>

class IndexedInterceptor : public NanObjectWrap {
  char buf[256];

 public:
  IndexedInterceptor() { std::strncpy(this->buf, "foo", sizeof (this->buf)); }
  static void Init (v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> NewInstance ();
  static NAN_METHOD(New);

  static NAN_INDEX_GETTER(PropertyGetter);
  static NAN_INDEX_SETTER(PropertySetter);
  static NAN_INDEX_ENUMERATOR(PropertyEnumerator);
  static NAN_INDEX_DELETER(PropertyDeleter);
  static NAN_INDEX_QUERY(PropertyQuery);
};

static NanPersistent<v8::FunctionTemplate> indexedinterceptors_constructor;

NAN_METHOD(CreateNew) {
  info.GetReturnValue().Set(IndexedInterceptor::NewInstance());
}

void IndexedInterceptor::Init(v8::Handle<v8::Object> target) {
  v8::Local<v8::FunctionTemplate> tpl =
    NanNew<v8::FunctionTemplate>(IndexedInterceptor::New);
  indexedinterceptors_constructor.Reset(tpl);
  tpl->SetClassName(NanNew("IndexedInterceptor").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  v8::Local<v8::ObjectTemplate> inst = tpl->InstanceTemplate();

  NanSetIndexedPropertyHandler(
      inst
    , IndexedInterceptor::PropertyGetter
    , IndexedInterceptor::PropertySetter
    , IndexedInterceptor::PropertyQuery
    , IndexedInterceptor::PropertyDeleter
    , IndexedInterceptor::PropertyEnumerator);

  v8::Local<v8::Function> createnew =
    NanNew<v8::FunctionTemplate>(CreateNew)->GetFunction();
  NanSet(target, NanNew("create").ToLocalChecked(), createnew);
}

v8::Handle<v8::Value> IndexedInterceptor::NewInstance () {
  NanEscapableScope scope;
  v8::Local<v8::FunctionTemplate> constructorHandle =
      NanNew(indexedinterceptors_constructor);
  v8::Local<v8::Object> instance =
    constructorHandle->GetFunction()->NewInstance(0, NULL);
  return scope.Escape(instance);
}

NAN_METHOD(IndexedInterceptor::New) {
  IndexedInterceptor* interceptor = new IndexedInterceptor();
  interceptor->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}


NAN_INDEX_GETTER(IndexedInterceptor::PropertyGetter) {
  IndexedInterceptor* interceptor =
    NanObjectWrap::Unwrap<IndexedInterceptor>(info.This());
  if (index == 0) {
    info.GetReturnValue().Set(NanNew(interceptor->buf).ToLocalChecked());
  } else {
    info.GetReturnValue().Set(NanNew("bar").ToLocalChecked());
  }
}

NAN_INDEX_SETTER(IndexedInterceptor::PropertySetter) {
  IndexedInterceptor* interceptor =
    NanObjectWrap::Unwrap<IndexedInterceptor>(info.This());
  if (index == 0) {
    std::strncpy(
        interceptor->buf
      , *v8::String::Utf8Value(value)
      , sizeof (interceptor->buf));
    info.GetReturnValue().Set(info.This());
  } else {
    info.GetReturnValue().Set(info.This());
  }
}

NAN_INDEX_ENUMERATOR(IndexedInterceptor::PropertyEnumerator) {
  v8::Local<v8::Array> arr = NanNew<v8::Array>();
  NanSet(arr, 0, NanNew("whee").ToLocalChecked());
  info.GetReturnValue().Set(arr);
}

NAN_INDEX_DELETER(IndexedInterceptor::PropertyDeleter) {
  IndexedInterceptor* interceptor =
    NanObjectWrap::Unwrap<IndexedInterceptor>(info.This());
  std::strncpy(interceptor->buf, "goober", sizeof (interceptor->buf));
  info.GetReturnValue().Set(NanTrue());
}

NAN_INDEX_QUERY(IndexedInterceptor::PropertyQuery) {
  if (index == 1) {
    info.GetReturnValue().Set(NanNew<v8::Integer>(v8::DontEnum));
  }
}

NODE_MODULE(indexedinterceptors, IndexedInterceptor::Init)
