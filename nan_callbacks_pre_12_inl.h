/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_CALLBACKS_PRE_12_INL_H_
#define NAN_CALLBACKS_PRE_12_INL_H_

namespace imp {
template<typename T> class ReturnValueImp;
}  // end of namespace imp

template<typename T>
class ReturnValue {
  v8::Isolate *isolate_;
  v8::Persistent<T> *value_;
  friend class imp::ReturnValueImp<T>;

 public:
  template <class S>
  explicit inline ReturnValue(v8::Isolate *isolate, v8::Persistent<S> *p) :
      isolate_(isolate), value_(p) {}
  template <class S>
  explicit inline ReturnValue(const ReturnValue<S>& that)
      : isolate_(that.isolate_), value_(that.value_) {
    TYPE_CHECK(T, S);
  }

  // Handle setters
  template <typename S> inline void Set(const v8::Local<S> &handle) {
    TYPE_CHECK(T, S);
    value_->Dispose();
    *value_ = v8::Persistent<T>::New(handle);
  }

  template <typename S> inline void Set(const Global<S> &handle) {
    TYPE_CHECK(T, S);
    value_->Dispose();
    *value_ = v8::Persistent<T>::New(handle.persistent);
    const_cast<Global<S> &>(handle).Reset();
  }

  // Fast primitive setters
  inline void Set(bool value) {
    TYPE_CHECK(T, v8::Boolean);
    value_->Dispose();
    *value_ = v8::Persistent<T>::New(v8::Boolean::New(value));
  }

  inline void Set(double i) {
    TYPE_CHECK(T, v8::Number);
    value_->Dispose();
    *value_ = v8::Persistent<T>::New(v8::Number::New(i));
  }

  inline void Set(int32_t i) {
    TYPE_CHECK(T, v8::Integer);
    value_->Dispose();
    *value_ = v8::Persistent<T>::New(v8::Int32::New(i));
  }

  inline void Set(uint32_t i) {
    TYPE_CHECK(T, v8::Integer);
    value_->Dispose();
    *value_ = v8::Persistent<T>::New(v8::Uint32::New(i));
  }

  // Fast JS primitive setters
  inline void SetNull() {
    TYPE_CHECK(T, v8::Primitive);
    value_->Dispose();
    *value_ = v8::Persistent<T>::New(v8::Null());
  }

  inline void SetUndefined() {
    TYPE_CHECK(T, v8::Primitive);
    value_->Dispose();
    *value_ = v8::Persistent<T>::New(v8::Undefined());
  }

  inline void SetEmptyString() {
    TYPE_CHECK(T, v8::String);
    value_->Dispose();
    *value_ = v8::Persistent<T>::New(v8::String::Empty());
  }

  // Convenience getter for isolate
  inline v8::Isolate *GetIsolate() const {
    return isolate_;
  }

  // Pointer setter: Uncompilable to prevent inadvertent misuse.
  template<typename S>
  inline void Set(S *whatever) { TYPE_CHECK(S*, v8::Primitive); }
};

template<typename T>
class FunctionCallbackInfo {
  const v8::Arguments &args_;
  v8::Local<v8::Value> data_;
  ReturnValue<T> return_value_;
  v8::Persistent<T> retval_;

 public:
  explicit inline FunctionCallbackInfo(
      const v8::Arguments &args
    , v8::Local<v8::Value> data) :
          args_(args)
        , data_(data)
        , return_value_(args.GetIsolate(), &retval_)
        , retval_(v8::Persistent<T>::New(v8::Undefined())) {}

  inline ~FunctionCallbackInfo() {
    retval_.Dispose();
    retval_.Clear();
  }

  inline ReturnValue<T> GetReturnValue() const {
    return ReturnValue<T>(return_value_);
  }

  inline v8::Local<v8::Function> Callee() const { return args_.Callee(); }
  inline v8::Local<v8::Value> Data() const { return data_; }
  inline v8::Local<v8::Object> Holder() const { return args_.Holder(); }
  inline bool IsConstructCall() const { return args_.IsConstructCall(); }
  inline int Length() const { return args_.Length(); }
  inline v8::Local<v8::Value> operator[](int i) const { return args_[i]; }
  inline v8::Local<v8::Object> This() const { return args_.This(); }
  inline v8::Isolate *GetIsolate() const { return args_.GetIsolate(); }


 protected:
  static const int kHolderIndex = 0;
  static const int kIsolateIndex = 1;
  static const int kReturnValueDefaultValueIndex = 2;
  static const int kReturnValueIndex = 3;
  static const int kDataIndex = 4;
  static const int kCalleeIndex = 5;
  static const int kContextSaveIndex = 6;
  static const int kArgsLength = 7;

 private:
  NAN_DISALLOW_ASSIGN_COPY_MOVE(FunctionCallbackInfo)
};

template<typename T>
class PropertyCallbackInfoBase {
  const v8::AccessorInfo &info_;
  const v8::Local<v8::Value> &data_;

 public:
  explicit inline PropertyCallbackInfoBase(
      const v8::AccessorInfo &info
    , const v8::Local<v8::Value> &data) :
          info_(info)
        , data_(data) {}

  inline v8::Isolate* GetIsolate() const { return info_.GetIsolate(); }
  inline v8::Local<v8::Value> Data() const { return data_; }
  inline v8::Local<v8::Object> This() const { return info_.This(); }
  inline v8::Local<v8::Object> Holder() const { return info_.Holder(); }

 protected:
  static const int kHolderIndex = 0;
  static const int kIsolateIndex = 1;
  static const int kReturnValueDefaultValueIndex = 2;
  static const int kReturnValueIndex = 3;
  static const int kDataIndex = 4;
  static const int kThisIndex = 5;
  static const int kArgsLength = 6;

 private:
  NAN_DISALLOW_ASSIGN_COPY_MOVE(PropertyCallbackInfoBase)
};

template<typename T>
class PropertyCallbackInfo : public PropertyCallbackInfoBase<T> {
  ReturnValue<T> return_value_;
  v8::Persistent<T> retval_;

 public:
  explicit inline PropertyCallbackInfo(
      const v8::AccessorInfo &info
    , const v8::Local<v8::Value> &data) :
          PropertyCallbackInfoBase<T>(info, data)
        , return_value_(info.GetIsolate(), &retval_)
        , retval_(v8::Persistent<T>::New(v8::Undefined())) {}

  inline ~PropertyCallbackInfo() {
    retval_.Dispose();
    retval_.Clear();
  }

  inline ReturnValue<T> GetReturnValue() const { return return_value_; }
};

template<>
class PropertyCallbackInfo<v8::Array> :
    public PropertyCallbackInfoBase<v8::Array> {
  ReturnValue<v8::Array> return_value_;
  v8::Persistent<v8::Array> retval_;

 public:
  explicit inline PropertyCallbackInfo(
      const v8::AccessorInfo &info
    , const v8::Local<v8::Value> &data) :
          PropertyCallbackInfoBase<v8::Array>(info, data)
        , return_value_(info.GetIsolate(), &retval_)
        , retval_(v8::Persistent<v8::Array>::New(v8::Local<v8::Array>())) {}

  inline ~PropertyCallbackInfo() {
    retval_.Dispose();
    retval_.Clear();
  }

  inline ReturnValue<v8::Array> GetReturnValue() const {
    return return_value_;
  }
};

template<>
class PropertyCallbackInfo<v8::Boolean> :
    public PropertyCallbackInfoBase<v8::Boolean> {
  ReturnValue<v8::Boolean> return_value_;
  v8::Persistent<v8::Boolean> retval_;

 public:
  explicit inline PropertyCallbackInfo(
      const v8::AccessorInfo &info
    , const v8::Local<v8::Value> &data) :
          PropertyCallbackInfoBase<v8::Boolean>(info, data)
        , return_value_(info.GetIsolate(), &retval_)
        , retval_(v8::Persistent<v8::Boolean>::New(v8::Local<v8::Boolean>())) {}

  inline ~PropertyCallbackInfo() {
    retval_.Dispose();
    retval_.Clear();
  }

  inline ReturnValue<v8::Boolean> GetReturnValue() const {
    return return_value_;
  }
};

template<>
class PropertyCallbackInfo<v8::Integer> :
    public PropertyCallbackInfoBase<v8::Integer> {
  ReturnValue<v8::Integer> return_value_;
  v8::Persistent<v8::Integer> retval_;

 public:
  explicit inline PropertyCallbackInfo(
      const v8::AccessorInfo &info
    , const v8::Local<v8::Value> &data) :
          PropertyCallbackInfoBase<v8::Integer>(info, data)
        , return_value_(info.GetIsolate(), &retval_)
        , retval_(v8::Persistent<v8::Integer>::New(v8::Local<v8::Integer>())) {}

  inline ~PropertyCallbackInfo() {
    retval_.Dispose();
    retval_.Clear();
  }

  inline ReturnValue<v8::Integer> GetReturnValue() const {
    return return_value_;
  }
};

namespace imp {
template<typename T>
class ReturnValueImp : public ReturnValue<T> {
 public:
  explicit ReturnValueImp(ReturnValue<T> that) :
      ReturnValue<T>(that) {}
  NAN_INLINE v8::Handle<T> Value() {
      return *ReturnValue<T>::value_;
  }
};

static
v8::Handle<v8::Value> FunctionCallbackWrapper(const v8::Arguments &args) {
  v8::Local<v8::Object> obj = args.Data().As<v8::Object>();
  FunctionWrapper *wrapper = static_cast<FunctionWrapper*>(
      obj->GetPointerFromInternalField(kFunctionIndex));
  FunctionCallbackInfo<v8::Value>
      cbinfo(args, obj->GetInternalField(kDataIndex));
  wrapper->callback(cbinfo);
  return ReturnValueImp<v8::Value>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Value> (*NativeFunction)(const v8::Arguments &);

static
v8::Handle<v8::Value> GetterCallbackWrapper(
    v8::Local<v8::String> property, const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Value>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  GetterWrapper *wrapper = static_cast<GetterWrapper*>(
      obj->GetPointerFromInternalField(kGetterIndex));
  wrapper->callback(property, cbinfo);
  return ReturnValueImp<v8::Value>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Value> (*NativeGetter)
    (v8::Local<v8::String>, const v8::AccessorInfo &);

static
void SetterCallbackWrapper(
    v8::Local<v8::String> property
  , v8::Local<v8::Value> value
  , const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<void>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  SetterWrapper *wrapper = static_cast<SetterWrapper*>(
      obj->GetPointerFromInternalField(kSetterIndex));
  wrapper->callback(property, value, cbinfo);
}

typedef void (*NativeSetter)
    (v8::Local<v8::String>, v8::Local<v8::Value>, const v8::AccessorInfo &);

static
v8::Handle<v8::Value> PropertyGetterCallbackWrapper(
    v8::Local<v8::String> property, const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Value>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  PropertyGetterWrapper *wrapper = static_cast<PropertyGetterWrapper*>(
      obj->GetPointerFromInternalField(kPropertyGetterIndex));
  wrapper->callback(property, cbinfo);
  return ReturnValueImp<v8::Value>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Value> (*NativePropertyGetter)
    (v8::Local<v8::String>, const v8::AccessorInfo &);

static
v8::Handle<v8::Value> PropertySetterCallbackWrapper(
    v8::Local<v8::String> property
  , v8::Local<v8::Value> value
  , const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Value>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  PropertySetterWrapper *wrapper = static_cast<PropertySetterWrapper*>(
      obj->GetPointerFromInternalField(kPropertySetterIndex));
  wrapper->callback(property, value, cbinfo);
  return ReturnValueImp<v8::Value>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Value> (*NativePropertySetter)
    (v8::Local<v8::String>, v8::Local<v8::Value>, const v8::AccessorInfo &);

static
v8::Handle<v8::Array> PropertyEnumeratorCallbackWrapper(
    const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Array>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  PropertyEnumeratorWrapper *wrapper = static_cast<PropertyEnumeratorWrapper*>(
      obj->GetPointerFromInternalField(kPropertyEnumeratorIndex));
  wrapper->callback(cbinfo);
  return ReturnValueImp<v8::Array>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Array> (*NativePropertyEnumerator)
    (const v8::AccessorInfo &);

static
v8::Handle<v8::Boolean> PropertyDeleterCallbackWrapper(
    v8::Local<v8::String> property
  , const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Boolean>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  PropertyDeleterWrapper *wrapper = static_cast<PropertyDeleterWrapper*>(
      obj->GetPointerFromInternalField(kPropertyDeleterIndex));
  wrapper->callback(property, cbinfo);
  return ReturnValueImp<v8::Boolean>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Boolean> (NativePropertyDeleter)
    (v8::Local<v8::String>, const v8::AccessorInfo &);

static
v8::Handle<v8::Integer> PropertyQueryCallbackWrapper(
    v8::Local<v8::String> property, const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Integer>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  PropertyQueryWrapper *wrapper = static_cast<PropertyQueryWrapper*>(
      obj->GetPointerFromInternalField(kPropertyQueryIndex));
  wrapper->callback(property, cbinfo);
  return ReturnValueImp<v8::Integer>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Integer> (*NativePropertyQuery)
    (v8::Local<v8::String>, const v8::AccessorInfo &);

static
v8::Handle<v8::Value> IndexGetterCallbackWrapper(
    uint32_t index, const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Value>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  IndexGetterWrapper *wrapper = static_cast<IndexGetterWrapper*>(
      obj->GetPointerFromInternalField(kIndexPropertyGetterIndex));
  wrapper->callback(index, cbinfo);
  return ReturnValueImp<v8::Value>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Value> (*NativeIndexGetter)
    (uint32_t, const v8::AccessorInfo &);

static
v8::Handle<v8::Value> IndexSetterCallbackWrapper(
    uint32_t index
  , v8::Local<v8::Value> value
  , const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Value>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  IndexSetterWrapper *wrapper = static_cast<IndexSetterWrapper*>(
      obj->GetPointerFromInternalField(kIndexPropertySetterIndex));
  wrapper->callback(index, value, cbinfo);
  return ReturnValueImp<v8::Value>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Value> (*NativeIndexSetter)
    (uint32_t, v8::Local<v8::Value>, const v8::AccessorInfo &);

static
v8::Handle<v8::Array> IndexEnumeratorCallbackWrapper(
    const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Array>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  IndexEnumeratorWrapper *wrapper = static_cast<IndexEnumeratorWrapper*>(
      obj->GetPointerFromInternalField(kIndexPropertyEnumeratorIndex));
  wrapper->callback(cbinfo);
  return ReturnValueImp<v8::Array>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Array> (*NativeIndexEnumerator)
    (const v8::AccessorInfo &);

static
v8::Handle<v8::Boolean> IndexDeleterCallbackWrapper(
    uint32_t index, const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Boolean>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  IndexDeleterWrapper *wrapper = static_cast<IndexDeleterWrapper*>(
      obj->GetPointerFromInternalField(kIndexPropertyDeleterIndex));
  wrapper->callback(index, cbinfo);
  return ReturnValueImp<v8::Boolean>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Boolean> (*NativeIndexDeleter)
    (uint32_t, const v8::AccessorInfo &);

static
v8::Handle<v8::Integer> IndexQueryCallbackWrapper(
    uint32_t index, const v8::AccessorInfo &info) {
  v8::Local<v8::Object> obj = info.Data().As<v8::Object>();
  PropertyCallbackInfo<v8::Integer>
      cbinfo(info, obj->GetInternalField(kDataIndex));
  IndexQueryWrapper *wrapper = static_cast<IndexQueryWrapper*>(
      obj->GetPointerFromInternalField(kIndexPropertyQueryIndex));
  wrapper->callback(index, cbinfo);
  return ReturnValueImp<v8::Integer>(cbinfo.GetReturnValue()).Value();
}

typedef v8::Handle<v8::Integer> (*NativeIndexQuery)
    (uint32_t, const v8::AccessorInfo &);
}  // end of namespace imp

#endif  // NAN_CALLBACKS_PRE_12_INL_H_
