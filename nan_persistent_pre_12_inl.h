/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_PERSISTENT_PRE_12_INL_H_
#define NAN_PERSISTENT_PRE_12_INL_H_

template<typename T>
class NanPersistentBase {
  v8::Persistent<T> persistent;
  template<typename U, typename M>
  friend v8::Local<U> NanNew(const NanPersistent<U, M> &p);
  template<typename S> friend class NanReturnValue;

 public:
  NAN_INLINE NanPersistentBase() :
      persistent() {}

  NAN_INLINE void Reset() {
    persistent.Dispose();
    persistent.Clear();
  }

  template<typename S>
  NAN_INLINE void Reset(const v8::Handle<S> &other) {
    TYPE_CHECK(T, S);

    if (!persistent.IsEmpty()) {
      persistent.Dispose();
    }

    if (other.IsEmpty()) {
      persistent.Clear();
    } else {
      persistent = v8::Persistent<T>::New(other);
    }
  }

  template<typename S>
  NAN_INLINE void Reset(const NanPersistentBase<S> &other) {
    TYPE_CHECK(T, S);

    if (!persistent.IsEmpty()) {
      persistent.Dispose();
    }

    if (other.IsEmpty()) {
      persistent.Clear();
    } else {
      persistent = v8::Persistent<T>::New(other.persistent);
    }
  }

  NAN_INLINE bool IsEmpty() const { return persistent.IsEmpty(); }

  NAN_INLINE void Empty() { persistent.Clear(); }

  template<typename S>
  NAN_INLINE bool operator==(const NanPersistentBase<S> &that) {
    return this->persistent == that.persistent;
  }

  template<typename S>
  NAN_INLINE bool operator==(const v8::Handle<S> &that) {
    return this->persistent == that;
  }

  template<typename S>
  NAN_INLINE bool operator!=(const NanPersistentBase<S> &that) {
    return !operator==(that);
  }

  template<typename S>
  NAN_INLINE bool operator!=(const v8::Handle<S> &that) {
    return !operator==(that);
  }

  template<typename P>
  NAN_INLINE void SetWeak(
    P *parameter
    , typename NanWeakCallbackInfo<P>::Callback callback
    , NanWeakCallbackType type);

  NAN_INLINE void ClearWeak() { persistent.ClearWeak(); }

  NAN_INLINE void MarkIndependent() { persistent.MarkIndependent(); }

  NAN_INLINE bool IsIndependent() const { return persistent.IsIndependent(); }

  NAN_INLINE bool IsNearDeath() const { return persistent.IsNearDeath(); }

  NAN_INLINE bool IsWeak() const { return persistent.IsWeak(); }

 private:
  NAN_INLINE explicit NanPersistentBase(v8::Persistent<T> that) :
      persistent(that) { }
  NAN_INLINE explicit NanPersistentBase(T *val) : persistent(val) {}
  template<typename S, typename M> friend class NanPersistent;
  template<typename S> friend class NanGlobal;
};

template<typename T>
class NanNonCopyablePersistentTraits {
 public:
  typedef NanPersistent<T, NanNonCopyablePersistentTraits<T> >
      NonCopyablePersistent;
  static const bool kResetInDestructor = false;
  template<typename S, typename M>
  NAN_INLINE static void Copy(const NanPersistent<S, M> &source,
                             NonCopyablePersistent *dest) {
    Uncompilable<v8::Object>();
  }

  template<typename O> NAN_INLINE static void Uncompilable() {
    TYPE_CHECK(O, v8::Primitive);
  }
};

template<typename T>
struct NanCopyablePersistentTraits {
  typedef NanPersistent<T, NanCopyablePersistentTraits<T> > CopyablePersistent;
  static const bool kResetInDestructor = true;
  template<typename S, typename M>
  static NAN_INLINE void Copy(const NanPersistent<S, M> &source,
                             CopyablePersistent *dest) {}
};

template<typename T, typename M> class NanPersistent :
    public NanPersistentBase<T> {
 public:
  NAN_INLINE NanPersistent() {}

  template<typename S> NAN_INLINE NanPersistent(v8::Handle<S> that)
      : NanPersistentBase<T>(v8::Persistent<T>::New(that)) {
    TYPE_CHECK(T, S);
  }

  NAN_INLINE NanPersistent(const NanPersistent &that) : NanPersistentBase<T>() {
    Copy(that);
  }

  template<typename S, typename M2>
  NAN_INLINE NanPersistent(const NanPersistent<S, M2> &that) :
      NanPersistentBase<T>() {
    Copy(that);
  }

  NAN_INLINE NanPersistent &operator=(const NanPersistent &that) {
    Copy(that);
    return *this;
  }

  template <class S, class M2>
  NAN_INLINE NanPersistent &operator=(const NanPersistent<S, M2> &that) {
    Copy(that);
    return *this;
  }

  NAN_INLINE ~NanPersistent() {
    if (M::kResetInDestructor) this->Reset();
  }

 private:
  NAN_INLINE T *operator*() const { return *NanPersistentBase<T>::persistent; }

  template<typename S, typename M2>
  NAN_INLINE void Copy(const NanPersistent<S, M2> &that) {
    TYPE_CHECK(T, S);

    this->Reset();

    if (!that.IsEmpty()) {
      this->persistent = v8::Persistent<T>::New(that.persistent);
      M::Copy(that, this);
    }
  }
};

template<typename T>
class NanGlobal : public NanPersistentBase<T> {
  struct RValue {
    NAN_INLINE explicit RValue(NanGlobal* obj) : object(obj) {}
    NanGlobal* object;
  };

 public:
  NAN_INLINE NanGlobal() : NanPersistentBase<T>(0) { }

  template <typename S>
  NAN_INLINE NanGlobal(v8::Handle<S> that)
      : NanPersistentBase<T>(v8::Persistent<T>::New(that)) {
    TYPE_CHECK(T, S);
  }

  template <typename S>
  NAN_INLINE NanGlobal(const NanPersistentBase<S> &that)
    : NanPersistentBase<T>(that) {
    TYPE_CHECK(T, S);
  }
  /**
   * Move constructor.
   */
  NAN_INLINE NanGlobal(RValue rvalue)
    : NanPersistentBase<T>(rvalue.object.persistent) {
    rvalue.object->Reset();
  }
  NAN_INLINE ~NanGlobal() { this->Reset(); }
  /**
   * Move via assignment.
   */
  template<typename S>
  NAN_INLINE NanGlobal &operator=(NanGlobal<S> rhs) {
    TYPE_CHECK(T, S);
    this->Reset(rhs.persistent);
    rhs.Reset();
    return *this;
  }
  /**
   * Cast operator for moves.
   */
  NAN_INLINE operator RValue() { return RValue(this); }
  /**
   * Pass allows returning uniques from functions, etc.
   */
  NanGlobal Pass() { return NanGlobal(RValue(this)); }

 private:
  NanGlobal(NanGlobal &);
  void operator=(NanGlobal &);
  template<typename S> friend class NanReturnValue;
};

#endif  // NAN_PERSISTENT_PRE_12_INL_H_
