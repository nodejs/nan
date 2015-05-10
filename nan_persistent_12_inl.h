/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_PERSISTENT_12_INL_H_
#define NAN_PERSISTENT_12_INL_H_

template<typename T, typename M> class NanPersistent :
    public v8::Persistent<T, M> {
 public:
  NAN_INLINE NanPersistent() : v8::Persistent<T, M>() {}

  template<typename S> NAN_INLINE NanPersistent(v8::Handle<S> that) :
      v8::Persistent<T, M>(v8::Isolate::GetCurrent(), that) {}

  template<typename S, typename M2>
  NAN_INLINE NanPersistent(const v8::Persistent<S, M2> &that) :
      v8::Persistent<T, M2>(v8::Isolate::GetCurrent(), that) {}

  NAN_INLINE void Reset() { v8::PersistentBase<T>::Reset(); }

  template <typename S>
  NAN_INLINE void Reset(const v8::Handle<S> &other) {
    v8::PersistentBase<T>::Reset(v8::Isolate::GetCurrent(), other);
  }

  template <typename S>
  NAN_INLINE void Reset(const v8::PersistentBase<S> &other) {
    v8::PersistentBase<T>::Reset(v8::Isolate::GetCurrent(), other);
  }

  template<typename P>
  NAN_INLINE void SetWeak(
    P *parameter
    , typename NanWeakCallbackInfo<P>::Callback callback
    , NanWeakCallbackType type);

 private:
  NAN_INLINE T *operator*() const { return *NanPersistentBase<T>::persistent; }

  template<typename S, typename M2>
  NAN_INLINE void Copy(const NanPersistent<S, M2> &that) {
    TYPE_CHECK(T, S);

    this->Reset();

    if (!that.IsEmpty()) {
      this->Reset(that);
      M::Copy(that, this);
    }
  }
};

#endif  // NAN_PERSISTENT_12_INL_H_
