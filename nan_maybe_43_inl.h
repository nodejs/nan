/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_MAYBE_43_INL_H_
#define NAN_MAYBE_43_INL_H_

template<typename T>
using NanMaybeLocal = v8::MaybeLocal<T>;

template<typename T>
using NanMaybe = v8::Maybe<T>;

template<typename T>
NAN_INLINE NanMaybe<T> NanNothing() {
  return v8::Nothing<T>();
}

template<typename T>
NAN_INLINE NanMaybe<T> NanJust(const T& t) {
  return v8::Just<T>(t);
}

v8::Local<v8::Context> NanGetCurrentContext();

NAN_INLINE
NanMaybeLocal<v8::String> NanToDetailString(v8::Handle<v8::Value> val) {
  return val->ToDetailString(NanGetCurrentContext());
}

NAN_INLINE
NanMaybeLocal<v8::Uint32> NanToArrayIndex(v8::Handle<v8::Value> val) {
  return val->ToArrayIndex(NanGetCurrentContext());
}

NAN_INLINE
NanMaybe<bool> NanEquals(v8::Handle<v8::Value> a, v8::Handle<v8::Value>(b)) {
  return a->Equals(NanGetCurrentContext(), b);
}

NAN_INLINE
NanMaybeLocal<v8::Object> NanNewInstance(v8::Handle<v8::Function> h) {
  return NanMaybeLocal<v8::Object>(h->NewInstance(NanGetCurrentContext()));
}

NAN_INLINE
NanMaybeLocal<v8::Object> NanNewInstance(v8::Handle<v8::ObjectTemplate> h) {
  return NanMaybeLocal<v8::Object>(h->NewInstance(NanGetCurrentContext()));
}


NAN_INLINE NanMaybeLocal<v8::Function> NanGetFunction(
    v8::Handle<v8::FunctionTemplate> t) {
  return t->GetFunction(NanGetCurrentContext());
}

NAN_INLINE NanMaybe<bool> NanSet(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> key
  , v8::Handle<v8::Value> value) {
  return obj->Set(NanGetCurrentContext(), key, value);
}

NAN_INLINE NanMaybe<bool> NanSet(
    v8::Handle<v8::Object> obj
  , uint32_t index
  , v8::Handle<v8::Value> value) {
  return obj->Set(NanGetCurrentContext(), index, value);
}

NAN_INLINE NanMaybe<bool> NanForceSet(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> key
  , v8::Handle<v8::Value> value
  , v8::PropertyAttribute attribs = v8::None) {
  return obj->ForceSet(NanGetCurrentContext(), key, value, attribs);
}

NAN_INLINE NanMaybeLocal<v8::Value> NanGet(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> key) {
  return obj->Get(NanGetCurrentContext(), key);
}

NAN_INLINE
NanMaybeLocal<v8::Value> NanGet(v8::Handle<v8::Object> obj, uint32_t index) {
  return obj->Get(NanGetCurrentContext(), index);
}

NAN_INLINE v8::PropertyAttribute NanGetPropertyAttributes(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> key) {
  return obj->GetPropertyAttributes(NanGetCurrentContext(), key).FromJust();
}

NAN_INLINE NanMaybe<bool> NanHas(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return obj->Has(NanGetCurrentContext(), key);
}

NAN_INLINE NanMaybe<bool> NanHas(v8::Handle<v8::Object> obj, uint32_t index) {
  return obj->Has(NanGetCurrentContext(), index);
}

NAN_INLINE NanMaybe<bool> NanDelete(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return obj->Delete(NanGetCurrentContext(), key);
}

NAN_INLINE
NanMaybe<bool> NanDelete(v8::Handle<v8::Object> obj, uint32_t index) {
  return obj->Delete(NanGetCurrentContext(), index);
}

NAN_INLINE
NanMaybeLocal<v8::Array> NanGetPropertyNames(v8::Handle<v8::Object> obj) {
  return obj->GetPropertyNames(NanGetCurrentContext());
}

NAN_INLINE
NanMaybeLocal<v8::Array> NanGetOwnPropertyNames(v8::Handle<v8::Object> obj) {
  return obj->GetOwnPropertyNames(NanGetCurrentContext());
}

NAN_INLINE NanMaybe<bool> NanSetPrototype(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> prototype) {
  return obj->SetPrototype(NanGetCurrentContext(), prototype);
}

NAN_INLINE NanMaybeLocal<v8::String> NanObjectProtoToString(
    v8::Handle<v8::Object> obj) {
  return obj->ObjectProtoToString(NanGetCurrentContext());
}

NAN_INLINE NanMaybe<bool> NanHasOwnProperty(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return obj->HasOwnProperty(NanGetCurrentContext(), key);
}

NAN_INLINE NanMaybe<bool> NanHasRealNamedProperty(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return obj->HasRealNamedProperty(NanGetCurrentContext(), key);
}

NAN_INLINE NanMaybe<bool> NanHasRealIndexedProperty(
    v8::Handle<v8::Object> obj
  , uint32_t index) {
  return obj->HasRealIndexedProperty(NanGetCurrentContext(), index);
}

NAN_INLINE NanMaybe<bool> NanHasRealNamedCallbackProperty(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return obj->HasRealNamedCallbackProperty(NanGetCurrentContext(), key);
}

NAN_INLINE NanMaybeLocal<v8::Value> NanGetRealNamedPropertyInPrototypeChain(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return obj->GetRealNamedPropertyInPrototypeChain(NanGetCurrentContext(), key);
}

NAN_INLINE NanMaybeLocal<v8::Value> NanGetRealNamedProperty(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return obj->GetRealNamedProperty(NanGetCurrentContext(), key);
}

NAN_INLINE NanMaybeLocal<v8::Value> NanCallAsFunction(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Object> recv
  , int argc
  , v8::Handle<v8::Value> argv[]) {
  return obj->CallAsFunction(NanGetCurrentContext(), recv, argc, argv);
}

NAN_INLINE NanMaybeLocal<v8::Value> NanCallAsConstructor(
    v8::Handle<v8::Object> obj
  , int argc, v8::Local<v8::Value> argv[]) {
  return obj->CallAsConstructor(NanGetCurrentContext(), argc, argv);
}

NAN_INLINE
NanMaybeLocal<v8::String> NanGetSourceLine(v8::Handle<v8::Message> msg) {
  return msg->GetSourceLine(NanGetCurrentContext());
}

NAN_INLINE NanMaybe<int> NanGetLineNumber(v8::Handle<v8::Message> msg) {
  return msg->GetLineNumber(NanGetCurrentContext());
}

NAN_INLINE NanMaybe<int> NanGetStartColumn(v8::Handle<v8::Message> msg) {
  return msg->GetStartColumn(NanGetCurrentContext());
}

NAN_INLINE NanMaybe<int> NanGetEndColumn(v8::Handle<v8::Message> msg) {
  return msg->GetEndColumn(NanGetCurrentContext());
}

NAN_INLINE NanMaybeLocal<v8::Object> NanCloneElementAt(
    v8::Handle<v8::Array> array
  , uint32_t index) {
  return array->CloneElementAt(NanGetCurrentContext(), index);
}

#endif  // NAN_MAYBE_43_INL_H_
