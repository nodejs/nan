/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_MAYBE_PRE_43_INL_H_
#define NAN_MAYBE_PRE_43_INL_H_

template<typename T>
class NanMaybeLocal {
 public:
  NAN_INLINE NanMaybeLocal() : val_(v8::Local<T>()) {}

  template<typename S>
# if NODE_MODULE_VERSION >= NODE_0_12_MODULE_VERSION
  NAN_INLINE NanMaybeLocal(v8::Local<S> that) : val_(that) {}
# else
  NAN_INLINE NanMaybeLocal(v8::Local<S> that) :
      val_(*reinterpret_cast<v8::Local<T>*>(&that)) {}
# endif

  NAN_INLINE bool IsEmpty() { return val_->IsEmpty(); }

  template<typename S>
  NAN_INLINE bool ToLocal(v8::Local<S> *out) {
    *out = val_;
    return !IsEmpty();
  }

  NAN_INLINE v8::Local<T> ToLocalChecked() {
#if defined(V8_ENABLE_CHECKS)
    assert(!IsEmpty() && "ToLocalChecked is Empty");
#endif  // V8_ENABLE_CHECKS
    return val_;
  }

  template<typename S>
  NAN_INLINE v8::Local<S> FromMaybe(v8::Local<S> default_value) const {
    return IsEmpty() ? default_value : val_;
  }

 private:
  v8::Local<T> val_;
};

template<typename T>
class NanMaybe {
 public:
  NAN_INLINE bool IsNothing() const { return !has_value_; }
  NAN_INLINE bool IsJust() const { return has_value_; }

  NAN_INLINE T FromJust() const {
#if defined(V8_ENABLE_CHECKS)
    assert(IsJust() && "FromJust is Nothing");
#endif  // V8_ENABLE_CHECKS
    return value_;
  }

  NAN_INLINE T FromMaybe(const T& default_value) const {
    return has_value_ ? value_ : default_value;
  }

  NAN_INLINE bool operator==(const NanMaybe &other) const {
    return (IsJust() == other.IsJust()) &&
        (!IsJust() || FromJust() == other.FromJust());
  }

  NAN_INLINE bool operator!=(const NanMaybe &other) const {
    return !operator==(other);
  }

 private:
  NanMaybe() : has_value_(false) {}
  explicit NanMaybe(const T& t) : has_value_(true), value_(t) {}
  bool has_value_;
  T value_;

  template<typename U>
  friend NanMaybe<U> NanNothing();
  template<typename U>
  friend NanMaybe<U> NanJust(const U& u);
};

template<typename T>
inline NanMaybe<T> NanNothing() {
  return NanMaybe<T>();
}

template<typename T>
inline NanMaybe<T> NanJust(const T& t) {
  return NanMaybe<T>(t);
}

NAN_INLINE
NanMaybeLocal<v8::String> NanToDetailString(v8::Handle<v8::Value> val) {
  return NanMaybeLocal<v8::String>(val->ToDetailString());
}

NAN_INLINE
NanMaybeLocal<v8::Uint32> NanToArrayIndex(v8::Handle<v8::Value> val) {
  return NanMaybeLocal<v8::Uint32>(val->ToArrayIndex());
}

NAN_INLINE
NanMaybe<bool> NanEquals(v8::Handle<v8::Value> a, v8::Handle<v8::Value>(b)) {
  return NanJust<bool>(a->Equals(b));
}

NAN_INLINE
NanMaybeLocal<v8::Object> NanNewInstance(v8::Handle<v8::Function> h) {
  return NanMaybeLocal<v8::Object>(h->NewInstance());
}

NAN_INLINE
NanMaybeLocal<v8::Object> NanNewInstance(v8::Handle<v8::ObjectTemplate> h) {
  return NanMaybeLocal<v8::Object>(h->NewInstance());
}

NAN_INLINE
NanMaybeLocal<v8::Function> NanGetFunction(v8::Handle<v8::FunctionTemplate> t) {
  return NanMaybeLocal<v8::Function>(t->GetFunction());
}

NAN_INLINE NanMaybe<bool> NanSet(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> key
  , v8::Handle<v8::Value> value) {
  return NanJust<bool>(obj->Set(key, value));
}

NAN_INLINE NanMaybe<bool> NanSet(
    v8::Handle<v8::Object> obj
  , uint32_t index
  , v8::Handle<v8::Value> value) {
  return NanJust<bool>(obj->Set(index, value));
}

NAN_INLINE NanMaybe<bool> NanForceSet(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> key
  , v8::Handle<v8::Value> value
  , v8::PropertyAttribute attribs = v8::None) {
  return NanJust<bool>(obj->ForceSet(key, value, attribs));
}

NAN_INLINE NanMaybeLocal<v8::Value> NanGet(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> key) {
  return NanMaybeLocal<v8::Value>(obj->Get(key));
}

NAN_INLINE NanMaybeLocal<v8::Value> NanGet(
    v8::Handle<v8::Object> obj
  , uint32_t index) {
  return NanMaybeLocal<v8::Value>(obj->Get(index));
}

NAN_INLINE NanMaybe<v8::PropertyAttribute> NanGetPropertyAttributes(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> key) {
  return NanJust<v8::PropertyAttribute>(obj->GetPropertyAttributes(key));
}

NAN_INLINE NanMaybe<bool> NanHas(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return NanJust<bool>(obj->Has(key));
}

NAN_INLINE NanMaybe<bool> NanHas(
    v8::Handle<v8::Object> obj
  , uint32_t index) {
  return NanJust<bool>(obj->Has(index));
}

NAN_INLINE NanMaybe<bool> NanDelete(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return NanJust<bool>(obj->Delete(key));
}

NAN_INLINE NanMaybe<bool> NanDelete(
    v8::Handle<v8::Object> obj
  , uint32_t index) {
  return NanJust<bool>(obj->Delete(index));
}

NAN_INLINE
NanMaybeLocal<v8::Array> NanGetPropertyNames(v8::Handle<v8::Object> obj) {
  return NanMaybeLocal<v8::Array>(obj->GetPropertyNames());
}

NAN_INLINE
NanMaybeLocal<v8::Array> NanGetOwnPropertyNames(v8::Handle<v8::Object> obj) {
  return NanMaybeLocal<v8::Array>(obj->GetOwnPropertyNames());
}

NAN_INLINE NanMaybe<bool> NanSetPrototype(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Value> prototype) {
  return NanJust<bool>(obj->SetPrototype(prototype));
}

NAN_INLINE NanMaybeLocal<v8::String> NanObjectProtoToString(
    v8::Handle<v8::Object> obj) {
  return NanMaybeLocal<v8::String>(obj->ObjectProtoToString());
}

NAN_INLINE NanMaybe<bool> NanHasOwnProperty(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return NanJust<bool>(obj->HasOwnProperty(key));
}

NAN_INLINE NanMaybe<bool> NanHasRealNamedProperty(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return NanJust<bool>(obj->HasRealNamedProperty(key));
}

NAN_INLINE NanMaybe<bool> NanHasRealIndexedProperty(
    v8::Handle<v8::Object> obj
  , uint32_t index) {
  return NanJust<bool>(obj->HasRealIndexedProperty(index));
}

NAN_INLINE NanMaybe<bool> NanHasRealNamedCallbackProperty(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return NanJust<bool>(obj->HasRealNamedCallbackProperty(key));
}

NAN_INLINE NanMaybeLocal<v8::Value> NanGetRealNamedPropertyInPrototypeChain(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return NanMaybeLocal<v8::Value>(
      obj->GetRealNamedPropertyInPrototypeChain(key));
}

NAN_INLINE NanMaybeLocal<v8::Value> NanGetRealNamedProperty(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::String> key) {
  return NanMaybeLocal<v8::Value>(obj->GetRealNamedProperty(key));
}

NAN_INLINE NanMaybeLocal<v8::Value> NanCallAsFunction(
    v8::Handle<v8::Object> obj
  , v8::Handle<v8::Object> recv
  , int argc
  , v8::Handle<v8::Value> argv[]) {
  return NanMaybeLocal<v8::Value>(obj->CallAsFunction(recv, argc, argv));
}

NAN_INLINE NanMaybeLocal<v8::Value> NanCallAsConstructor(
    v8::Handle<v8::Object> obj
  , int argc
  , v8::Local<v8::Value> argv[]) {
  return NanMaybeLocal<v8::Value>(obj->CallAsConstructor(argc, argv));
}

NAN_INLINE
NanMaybeLocal<v8::String> NanGetSourceLine(v8::Handle<v8::Message> msg) {
  return NanMaybeLocal<v8::String>(msg->GetSourceLine());
}

NAN_INLINE NanMaybe<int> NanGetLineNumber(v8::Handle<v8::Message> msg) {
  return NanJust<int>(msg->GetLineNumber());
}

NAN_INLINE NanMaybe<int> NanGetStartColumn(v8::Handle<v8::Message> msg) {
  return NanJust<int>(msg->GetStartColumn());
}

NAN_INLINE NanMaybe<int> NanGetEndColumn(v8::Handle<v8::Message> msg) {
  return NanJust<int>(msg->GetEndColumn());
}

NAN_INLINE NanMaybeLocal<v8::Object> NanCloneElementAt(
    v8::Handle<v8::Array> array
  , uint32_t index) {
  return NanMaybeLocal<v8::Object>(array->CloneElementAt(index));
}

NAN_INLINE NanMaybeLocal<v8::Value> NanStackTrace(v8::TryCatch trycatch) {
  return NanMaybeLocal<v8::Value>(trycatch.StackTrace());
}

#endif  // NAN_MAYBE_PRE_43_INL_H_

