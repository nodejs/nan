/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_PERSISTENT_12_INL_H_
#define NAN_PERSISTENT_12_INL_H_

template<typename T>
class PersistentBase {
 public:
  inline void Reset() { persistent.Reset(); }

  template<typename S>
  inline void Reset(const v8::Local<S> &other) {
    persistent.Reset(v8::Isolate::GetCurrent(), other);
  }

  template<typename S>
  inline void Reset(const PersistentBase<S> &other) {
    persistent.Reset(v8::Isolate::GetCurrent(), other.persistent);
  }

  inline bool IsEmpty() const {
    return persistent.IsEmpty();
  }

  inline void Empty() {
#if NODE_MODULE_VERSION == NODE_0_12_MODULE_VERSION
    persistent_.ClearAndLeak();
#else
    persistent.Empty();
#endif
  }

  template<typename S>
  inline bool operator==(const PersistentBase<S> &that) const {
    return persistent == that.persistent;
  }

  template<typename S>
  inline bool operator==(const v8::Local<S> &that) const {
    return persistent == that;
  }

  template<typename S>
  inline bool operator!=(const PersistentBase<S> &that) const {
    return !operator==(that);
  }

  template<typename S>
  inline bool operator!=(const v8::Local<S> &that) const {
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
  struct BasePersistentTraits {
    static const bool kResetInDestructor = false;

    template<typename S, typename M>
    static inline void Copy(const v8::Persistent<S, M> &source,
        BasePersistentTraits *dest) {}
  };

  v8::Persistent<T, BasePersistentTraits> persistent_;
  v8::PersistentBase<T> &persistent;

  template<typename U>
  friend v8::Local<U> New(const PersistentBase<U> &p);
  template<typename U, typename M>
  friend v8::Local<U> New(const Persistent<U, M> &p);
  template<typename U>
  friend v8::Local<U> New(const Global<U> &p);
  template<typename S> friend class ReturnValue;
  template<typename S, typename M> friend class Persistent;
  template<typename S> friend class Global;
  friend class ObjectWrap;

  inline PersistentBase() : persistent(persistent_) {}
  inline PersistentBase(v8::Local<T> *other) :
      persistent_(v8::Isolate::GetCurrent(), *other), persistent(persistent_) {}
  inline PersistentBase(const v8::PersistentBase<T> &other) :
      persistent_(v8::Isolate::GetCurrent(),
          static_cast<const v8::Persistent<T, BasePersistentTraits>&>(other)),
      persistent(persistent_) {}
#if defined(V8_MAJOR_VERSION) && (V8_MAJOR_VERSION > 4 ||                      \
  (V8_MAJOR_VERSION == 4 && defined(V8_MINOR_VERSION) && V8_MINOR_VERSION >= 3))
  PersistentBase(const PersistentBase &) = delete;
  void operator=(const PersistentBase &) = delete;
#else
 private:
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
#endif
};

template<typename T>
class NonCopyablePersistentTraits {
 public:
  typedef Persistent<T, NonCopyablePersistentTraits<T> > NonCopyablePersistent;

  static const bool kResetInDestructor = false;

  template<typename S, typename M>
  static inline void Copy(const Persistent<S, M> &source,
      NonCopyablePersistent *dest) {
    Uncompilable<v8::Object>();
  }

  template<typename O>
  inline static void Uncompilable() {
    TYPE_CHECK(O, v8::Primitive);
  }
};

template<typename T>
class CopyablePersistentTraits {
 public:
  typedef Persistent<T, CopyablePersistentTraits<T> > CopyablePersistent;

  static const bool kResetInDestructor = true;

  template<typename S, typename M>
  static inline void Copy(const Persistent<S, M> &source,
      CopyablePersistent *dest) {}
};

template<typename T, typename M>
class Persistent : public PersistentBase<T> {
 public:
  inline Persistent() : PersistentBase<T>() {}

  template<typename S>
  inline explicit Persistent(v8::Local<S> that) :
      PersistentBase<T>(&that) { TYPE_CHECK(T, S); } 

  template<typename S>
  inline explicit Persistent(const PersistentBase<S> &that) :
      PersistentBase<T>(that) {
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

  template<typename S, typename M2>
  inline Persistent &operator=(const Persistent<S, M2> &that) {
    this->Copy(that);
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
      T::Cast(*reinterpret_cast<v8::Local<S>&>(that.persistent));
    }
#endif
    return reinterpret_cast<Persistent<T>&>(that);
  }

  template<typename S>
  inline Persistent<S> &As() {
    return Persistent<S>::Cast(*this);
  }

 private:
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

template<typename T>
class Global : public PersistentBase<T> {
 public:
  inline Global() : PersistentBase<T>() {}

  template<typename S>
  inline explicit Global(v8::Local<S> that) :
PersistentBase<T>(static_cast<v8::Local<T>*>(&that)) {}

  inline ~Global() { this->Reset(); }

#if defined(V8_MAJOR_VERSION) && (V8_MAJOR_VERSION > 4 ||                      \
  (V8_MAJOR_VERSION == 4 && defined(V8_MINOR_VERSION) && V8_MINOR_VERSION >= 3))
  template<typename S>
  inline explicit Global(const Global<S> &other) :
      Global<T>(static_cast<const PersistentBase<S> &>(other)) {}

  template<typename S>
  inline explicit Global(const PersistentBase<S> &other) : PersistentBase<T>() {
    fprintf(stderr, "copy\n");
    static_cast<v8::Global<T>&>(this->persistent) =
        v8::Global<T>(v8::Isolate::GetCurrent(), other.persistent);
  }

  template<typename S>
  inline Global(PersistentBase<S> &&other) :
    Global<T>(static_cast<Global<S>&&>(other)) {}

  template<typename S>
  inline Global(Global<S> &other) : Global<T>(static_cast<Global<S>&&>(other)) {
  }

  template<typename S>
  inline Global(Global<S> &&other) : PersistentBase<T>() {
    fprintf(stderr, "move\n");
    TYPE_CHECK(T, S);
    static_cast<v8::Global<T>&>(this->persistent) =
        reinterpret_cast<v8::Global<T>&&>(other.persistent);
  }

  template<typename S>
  inline Global(const Global<S> &&other) :
      Global<T>(static_cast<const PersistentBase<S>&>(other)) {}


  inline Global(const Global&) = delete;
  inline Global &operator=(const Global &) = delete;

  template<typename S>
  inline Global &operator=(Global<S> &other) {
    return operator=(static_cast<Global<S>&&>(other));
  }

  template<typename S>
  inline Global &operator=(Global<S> &&other) {
    fprintf(stderr, "move=\n");
    TYPE_CHECK(T, S);
    static_cast<v8::Global<T>&>(this->persistent) =
        // work around V8 bug comparing different pointer types
        reinterpret_cast<v8::Global<T>&&>(other.persistent);
    return *this;
  }

  inline Global Pass() { return static_cast<Global&&>(*this); }
#else
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
        PersistentBase<T>() {                                                  \
      fprintf(stderr, "copy\n");                                               \
      static_cast<v8::UniquePersistent<T>&>(this->persistent) =                \
          v8::UniquePersistent<T>(v8::Isolate::GetCurrent(),                   \
              other.get().persistent);                                         \
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
  X(UnboundScript)

#undef X

#define X(TYPE)                                                                \
    inline Global(PersistentBase<v8::TYPE>::TemporaryPersistentBase other) :   \
        PersistentBase<T>() {                                                  \
      fprintf(stderr, "move\n");                                               \
      static_cast<v8::UniquePersistent<T>&>(this->persistent) =                \
          static_cast<v8::UniquePersistent<v8::TYPE>&>(other.get().persistent).Pass();\
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
  X(UnboundScript)

#undef X

  template<typename S>
  inline explicit Global(PersistentBase<S> &other) : PersistentBase<T>() {
    fprintf(stderr, "copy\n");
    static_cast<v8::UniquePersistent<T>&>(this->persistent) =
        v8::UniquePersistent<T>(v8::Isolate::GetCurrent(),
            other.persistent);
  }

  template<typename S>
  inline Global(Global<S> &other) : PersistentBase<T>() {
    fprintf(stderr, "move\n");
    static_cast<v8::UniquePersistent<T>&>(this->persistent) =
static_cast<v8::UniquePersistent<S>&>(other.persistent).Pass();
  }

  inline Global(Global &other) : PersistentBase<T>() {
    fprintf(stderr, "move\n");
    static_cast<v8::UniquePersistent<T>&>(this->persistent) =
static_cast<v8::UniquePersistent<T>&>(other.persistent).Pass();
  } 

#define X(TYPE) \
  inline Global &operator=(typename PersistentBase<v8::TYPE>::TemporaryPersistentBase other) { \
    fprintf(stderr, "move=\n"); \
    static_cast<v8::UniquePersistent<T>&>(this->persistent) = \
static_cast<v8::UniquePersistent<v8::TYPE>&>(other.get().persistent).Pass(); \
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
  X(UnboundScript)

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
#endif
};

#endif  // NAN_PERSISTENT_12_INL_H_

