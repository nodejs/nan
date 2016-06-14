/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_PERSISTENT_12_INL_H_
#define NAN_PERSISTENT_12_INL_H_

template<typename T, typename M> class Persistent :
    public v8::Persistent<T, M> {
 public:
  inline Persistent() : v8::Persistent<T, M>() {}

  template<typename S> inline explicit  Persistent(v8::Local<S> that) :
      v8::Persistent<T, M>(v8::Isolate::GetCurrent(), that) {}

  template<typename S, typename M2>
  inline explicit Persistent(const v8::Persistent<S, M2> &that) :
      v8::Persistent<T, M2>(v8::Isolate::GetCurrent(), that) {}

  inline void Reset() { v8::PersistentBase<T>::Reset(); }

  template<typename S>
  inline void Reset(const v8::Local<S> &other) {
    v8::PersistentBase<T>::Reset(v8::Isolate::GetCurrent(), other);
  }

  template<typename S>
  inline void Reset(const v8::PersistentBase<S> &other) {
    v8::PersistentBase<T>::Reset(v8::Isolate::GetCurrent(), other);
  }

#if NODE_MODULE_VERSION == NODE_0_12_MODULE_VERSION
  inline void Empty() {
    v8::Persistent<T, M>::ClearAndLeak();
  }
#endif

  template<typename P>
  inline void SetWeak(
    P *parameter
    , typename WeakCallbackInfo<P>::Callback callback
    , WeakCallbackType type);

 private:
  inline T *operator*() const { return *PersistentBase<T>::persistent; }

  template<typename S, typename M2>
  inline void Copy(const Persistent<S, M2> &that) {
    TYPE_CHECK(T, S);

    this->Reset();

    if (!that.IsEmpty()) {
      this->Reset(that);
      M::Copy(that, this);
    }
  }
};

#if defined(V8_MAJOR_VERSION) && (V8_MAJOR_VERSION > 4 ||                      \
  (V8_MAJOR_VERSION == 4 && defined(V8_MINOR_VERSION) && V8_MINOR_VERSION >= 3))
template<typename T>
class Global : public v8::Global<T> {
 public:
  inline Global() : v8::Global<T>() {}

  template<typename S> inline explicit Global(v8::Local<S> that) :
    v8::Global<T>(v8::Isolate::GetCurrent(), that) {}

  template<typename S>
  inline explicit Global(const v8::PersistentBase<S> &that) :
      v8::Global<S>(v8::Isolate::GetCurrent(), that) {}

  inline void Reset() { v8::PersistentBase<T>::Reset(); }

  template <typename S>
  inline void Reset(const v8::Local<S> &other) {
    v8::PersistentBase<T>::Reset(v8::Isolate::GetCurrent(), other);
  }

  template <typename S>
  inline void Reset(const v8::PersistentBase<S> &other) {
    v8::PersistentBase<T>::Reset(v8::Isolate::GetCurrent(), other);
  }

  Global Pass() { return static_cast<Global&&>(*this); }  // NOLINT(build/c++11)

  template<typename P>
  inline void SetWeak(
    P *parameter
    , typename WeakCallbackInfo<P>::Callback callback
    , WeakCallbackType type) {
    reinterpret_cast<Persistent<T>*>(this)->SetWeak(
        parameter, callback, type);
  }
};
#else
template<typename T>
class Global : public v8::UniquePersistent<T> {
  struct RValue {
    inline explicit RValue(v8::UniquePersistent<T> *obj) : object_(obj) {}
    v8::UniquePersistent<T> *object_;
  };

 public:
  inline Global() : v8::UniquePersistent<T>() {}

  template<typename S> inline explicit Global(v8::Local<S> that) :
    v8::UniquePersistent<T>(v8::Isolate::GetCurrent(), that) {}

  template<typename S>
  inline explicit Global(const v8::PersistentBase<S> &that) :
      v8::UniquePersistent<S>(v8::Isolate::GetCurrent(), that) {}

  inline void Reset() { v8::PersistentBase<T>::Reset(); }

  template<typename S>
  inline void Reset(const v8::Local<S> &other) {
    v8::PersistentBase<T>::Reset(v8::Isolate::GetCurrent(), other);
  }

  template<typename S>
  inline void Reset(const v8::PersistentBase<S> &other) {
    v8::PersistentBase<T>::Reset(v8::Isolate::GetCurrent(), other);
  }

#if NODE_MODULE_VERSION == NODE_0_12_MODULE_VERSION
  inline void Empty() {
    reinterpret_cast<v8::Persistent<T>*>(this)->ClearAndLeak();
  }
#endif

  inline Global(RValue rvalue) {  // NOLINT(runtime/explicit)
    std::memcpy(this, rvalue.object_, sizeof (*rvalue.object_));
# if NODE_MODULE_VERSION > NODE_0_12_MODULE_VERSION
    rvalue.object_->Empty();
# else
    reinterpret_cast<v8::Persistent<T>*>(rvalue.object_)->ClearAndLeak();
# endif
  }

  template<typename S>
  inline Global &operator=(v8::UniquePersistent<S> other) {
    return reinterpret_cast<Global>(v8::UniquePersistent<S>::operator=(other));
  }

  inline operator RValue() { return RValue(this); }

  Global Pass() { return Global(RValue(this)); }

  template<typename P>
  inline void SetWeak(
    P *parameter
    , typename WeakCallbackInfo<P>::Callback callback
    , WeakCallbackType type) {
    reinterpret_cast<Persistent<T>*>(this)->SetWeak(
        parameter, callback, type);
  }
};
#endif

#endif  // NAN_PERSISTENT_12_INL_H_
