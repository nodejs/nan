/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_PERSISTENT_PRE_12_INL_H_
#define NAN_PERSISTENT_PRE_12_INL_H_

template<typename T>
class PersistentBase {
  v8::Persistent<T> persistent;
  template<typename U>
  friend v8::Local<U> New(const PersistentBase<U> &p);
  template<typename U, typename M>
  friend v8::Local<U> New(const Persistent<U, M> &p);
  template<typename U>
  friend v8::Local<U> New(const Global<U> &p);
  template<typename S> friend class ReturnValue;

 public:
  inline PersistentBase() :
      persistent() {}

  inline void Reset() {
    persistent.Dispose();
    persistent.Clear();
  }

  template<typename S>
  inline void Reset(const v8::Local<S> &other) {
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
  inline void Reset(const PersistentBase<S> &other) {
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

  inline bool IsEmpty() const { return persistent.IsEmpty(); }

  inline void Empty() { persistent.Clear(); }

  template<typename S>
  inline bool operator==(const PersistentBase<S> &that) {
    return this->persistent == that.persistent;
  }

  template<typename S>
  inline bool operator==(const v8::Local<S> &that) {
    return this->persistent == that;
  }

  template<typename S>
  inline bool operator!=(const PersistentBase<S> &that) {
    return !operator==(that);
  }

  template<typename S>
  inline bool operator!=(const v8::Local<S> &that) {
    return !operator==(that);
  }

  template<typename P>
  inline void SetWeak(
    P *parameter
    , typename WeakCallbackInfo<P>::Callback callback
    , WeakCallbackType type);

  inline void ClearWeak() { persistent.ClearWeak(); }

  inline void MarkIndependent() { persistent.MarkIndependent(); }

  inline bool IsIndependent() const { return persistent.IsIndependent(); }

  inline bool IsNearDeath() const { return persistent.IsNearDeath(); }

  inline bool IsWeak() const { return persistent.IsWeak(); }

 private:
  inline explicit PersistentBase(v8::Persistent<T> that) :
      persistent(that) { }
  inline explicit PersistentBase(T *val) : persistent(val) {}
  template<typename S, typename M> friend class Persistent;
  template<typename S> friend class Global;
  friend class ObjectWrap;

  class ConstantPersistentBase {
   public:
    inline explicit ConstantPersistentBase(const PersistentBase &obj) :
        obj_(&obj) {}
    inline const PersistentBase &get() const { return *obj_; }

   private:
    const PersistentBase *obj_;
  };
  class TemporaryPersistentBase : private ConstantPersistentBase {
   public:
    inline explicit TemporaryPersistentBase(PersistentBase &obj) :
        ConstantPersistentBase(obj) {}
    inline PersistentBase &get() const {
      return const_cast<PersistentBase &>(ConstantPersistentBase::get());
    }
  };
 public:
  inline operator ConstantPersistentBase() const { return
ConstantPersistentBase(static_cast<const
PersistentBase &>(*this)); }
  inline operator TemporaryPersistentBase() { return
TemporaryPersistentBase(static_cast<PersistentBase &>(*this)); }

 private:
  PersistentBase(PersistentBase &);
  void operator=(PersistentBase &);
};

template<typename T>
class NonCopyablePersistentTraits {
 public:
  typedef Persistent<T, NonCopyablePersistentTraits<T> >
      NonCopyablePersistent;
  static const bool kResetInDestructor = false;
  template<typename S, typename M>
  inline static void Copy(const Persistent<S, M> &source,
                             NonCopyablePersistent *dest) {
    Uncompilable<v8::Object>();
  }

  template<typename O> inline static void Uncompilable() {
    TYPE_CHECK(O, v8::Primitive);
  }
};

template<typename T>
struct CopyablePersistentTraits {
  typedef Persistent<T, CopyablePersistentTraits<T> > CopyablePersistent;
  static const bool kResetInDestructor = true;
  template<typename S, typename M>
  static inline void Copy(const Persistent<S, M> &source,
                             CopyablePersistent *dest) {}
};

template<typename T, typename M> class Persistent :
    public PersistentBase<T> {
 public:
  inline Persistent() {}

  template<typename S> inline explicit Persistent(v8::Local<S> that)
      : PersistentBase<T>(v8::Persistent<T>::New(that)) {
    TYPE_CHECK(T, S);
  }

  template<typename S>
  inline explicit Persistent(const PersistentBase<S> &that) :
    PersistentBase<T>(v8::Persistent<T>::New(*that)) {
    TYPE_CHECK(T, S);
  }

  inline Persistent(const Persistent &that) : PersistentBase<T>() {
    Copy(that);
  }

  template<typename S, typename M2>
  inline Persistent(const Persistent<S, M2> &that) :
    PersistentBase<T>() {
    Copy(that);
  } 

  inline Persistent &operator=(const Persistent &that) {
    Copy(that);
    return *this;
  }

  template <class S, class M2>
  inline Persistent &operator=(const Persistent<S, M2> &that) {
    Copy(that);
    return *this;
  }

  inline ~Persistent() {
    if (M::kResetInDestructor) {
      this->Reset();
    }
  }

  template<typename S>
  inline static Persistent<T> &Cast(Persistent<S> &that) {
#ifdef V8_ENABLE_CHECKS
    if (!that.IsEmpty()) {
      T::Cast(*that);
    }
#endif
    return reinterpret_cast<Persistent<T>&>(that);
  }

 private:
  inline T *operator*() const { return *PersistentBase<T>::persistent; }

  template<typename S, typename M2>
  inline void Copy(const Persistent<S, M2> &that) {
    TYPE_CHECK(T, S);

    this->Reset();

    if (!that.IsEmpty()) {
      this->persistent = v8::Persistent<T>::New(that.persistent);
      M::Copy(that, this);
    }
  }
};

template<typename T>
class Global : public PersistentBase<T> {
 public:
  inline Global() : PersistentBase<T>() { }

  template <typename S>
  inline explicit Global(v8::Local<S> that)
      : PersistentBase<T>(v8::Persistent<T>::New(that)) {
    TYPE_CHECK(T, S);
  }

  template <typename S>
  inline explicit Global(const PersistentBase<S> &that) :
      PersistentBase<T>(v8::Persistent<T>::New(that.persistent)) {
    TYPE_CHECK(T, S);
  }

  inline ~Global() { this->Reset(); }

 private:
  template<typename S> class ConstantGlobal : private
PersistentBase<S>::ConstantPersistentBase {
   public:
    explicit ConstantGlobal(const Global<S> &obj) :
        PersistentBase<S>::ConstantPersistentBase(obj) {}
    inline const Global<S> &get() {
      return PersistentBase<S>::ConstantPersistentBase::get();
    }
  };

 public:
  inline operator ConstantGlobal<T>() const {
    return ConstantGlobal<T>(static_cast<const Global<T> &>(*this));
  }
 
#define X(TYPE)                                                                \
    inline explicit Global(PersistentBase<v8::TYPE>::ConstantPersistentBase other) :    \
        PersistentBase<T>(v8::Persistent<T>::New(other.get().persistent)) {    \
      fprintf(stderr, "copy\n");                                               \
    }

  X(Array)
  X(Boolean)
  X(BooleanObject)
  X(Context)
  X(Date)
  X(External)
  X(Function)
  X(FunctionTemplate)
  X(Int32)
  X(Integer)
  X(Number)
  X(NumberObject)
  X(Object)
  X(ObjectTemplate)
  X(RegExp)
  X(Script)
  X(Signature)
  X(String)
  X(StringObject)
  X(Uint32)

#undef X

#define X(TYPE)                                                                \
    inline Global(PersistentBase<v8::TYPE>::TemporaryPersistentBase other) :   \
        PersistentBase<T>(reinterpret_cast<T*>(*other.get().persistent)) {     \
      TYPE_CHECK(T, v8::TYPE);                                                 \
      fprintf(stderr, "move\n");                                               \
      other.get().Empty();                                                     \
    }

  X(Array)
  X(Boolean)
  X(BooleanObject)
  X(Context)
  X(Date)
  X(External)
  X(Function)
  X(FunctionTemplate)
  X(Int32)
  X(Integer)
  X(Number)
  X(NumberObject)
  X(Object)
  X(ObjectTemplate)
  X(RegExp)
  X(Script)
  X(Signature)
  X(String)
  X(StringObject)
  X(Uint32)

#undef X

  template<typename S>
  inline explicit Global(PersistentBase<S> &other) :
      PersistentBase<T>(v8::Persistent<T>::New(other.persistent)) {
    fprintf(stderr, "copy\n");
  }

  template<typename S>
  inline Global(Global<S> &other) :
      PersistentBase<T>(reinterpret_cast<T*>(*other.persistent)) {
    TYPE_CHECK(T, S);
    fprintf(stderr, "move\n");
    other.Empty();
  }

  inline Global(Global &other) : PersistentBase<T>(other.persistent) {
    fprintf(stderr, "move\n");
    other.Empty();
  } 

#define X(TYPE) \
  inline Global &operator=(typename PersistentBase<v8::TYPE>::TemporaryPersistentBase other) { \
    fprintf(stderr, "move=\n"); \
    this->Reset(); \
    this->persistent = other.get().persistent; \
    other.get().Empty(); \
    return *this; \
  }

  X(Array)
  X(Boolean)
  X(BooleanObject)
  X(Context)
  X(Date)
  X(External)
  X(Function)
  X(FunctionTemplate)
  X(Int32)
  X(Integer)
  X(Number)
  X(NumberObject)
  X(Object)
  X(ObjectTemplate)
  X(RegExp)
  X(Script)
  X(Signature)
  X(String)
  X(StringObject)
  X(Uint32)

#undef X

  template<typename S>
  inline Global &operator=(Global<S> &other) {
    return
operator=(typename PersistentBase<S>::TemporaryPersistentBase(static_cast<PersistentBase<S>
&>(other)));
  }

  inline Global Pass() {
    return Global(typename
PersistentBase<T>::TemporaryPersistentBase(static_cast<PersistentBase<T>
&>(*this))); 
  }
};

#endif  // NAN_PERSISTENT_PRE_12_INL_H_
