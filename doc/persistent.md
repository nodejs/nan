# Persistent
An object reference that is independent of any handle scope. Where a Local
handle only lives as long as the `HandleScope` in which it was allocated,
a `Persistent` handle remains valid until it is explicitly disposed.

## PersistentBase
A persistent handle contains a reference to a storage cell within
the v8 engine which holds an object value and which is updated by
the garbage collector whenever the object is moved.  A new storage
cell can be created using the constructor or `PersistentBase::Reset`.
Existing handles can be disposed using `PersistentBase::Reset`.
```c++
template<typename T>
class PersistentBase {
 public:
  /**
   * If non-empty, destroy the underlying storage cell
   * IsEmpty() will return true after this call.
   */
  void Reset();

  /**
   * If non-empty, destroy the underlying storage cell
   * and create a new one with the contents of other if other is non empty
   */
  template<typename S>
  void Reset(const v8::Handle<S> &other);

  /**
   * If non-empty, destroy the underlying storage cell
   * and create a new one with the contents of other if other is non empty
   */
  template<typename S>
  void Reset(const PersistentBase<S> &other);

  bool IsEmpty();

  void Empty();

  template<typename S>
  bool operator==(const PersistentBase<S> &that);

  template<typename S>
  bool operator==(const v8::Handle<S> &that);

  template<typename S>
  bool operator!=(const PersistentBase<S> &that);

  template<typename S>
  bool operator!=(const v8::Handle<S> &that);

   /**
   *  Install a finalization callback on this object.
   *  NOTE: There is no guarantee as to *when* or even *if* the callback is
   *  invoked. The invocation is performed solely on a best effort basis.
   *  As always, GC-based finalization should *not* be relied upon for any
   *  critical form of resource management! At the moment you can either
   *  specify a parameter for the callback or the location of two internal
   *  fields in the dying object.
   */
  template<typename P>
  void SetWeak(
      P *parameter,
      typename WeakCallbackInfo<P>::Callback callback,
      WeakCallbackType type);

  void ClearWeak();

  /**
   * Marks the reference to this object independent. Garbage collector is free
   * to ignore any object groups containing this object. Weak callback for an
   * independent handle should not assume that it will be preceded by a global
   * GC prologue callback or followed by a global GC epilogue callback.
   */
  void MarkIndependent() const;

  bool IsIndependent() const;

  /** Checks if the handle holds the only reference to an object. */
  bool IsNearDeath() const;

  /** Returns true if the handle's reference is weak.  */
  bool IsWeak() const
};
```

## NonCopyablePersistentTraits
Default traits for `Persistent`. This class does not allow use of the copy
constructor or assignment operator. At present `kResetInDestructor` is not
set, but that will change in a future version.
```c++
template<typename T>
class NonCopyablePersistentTraits {
 public:
  typedef Persistent<T, NonCopyablePersistentTraits<T> > NonCopyablePersistent;
  static const bool kResetInDestructor = false;
  template<typename S, typename M>
};
```

## CopyablePersistentTraits
Helper class traits to allow copying and assignment of `Persistent`.
This will clone the contents of storage cell, but not any of the flags, etc.
```c++
template<typename T>
class CopyablePersistentTraits {
 public:
  typedef Persistent<T, CopyablePersistentTraits<T> > CopyablePersistent;
  static const bool kResetInDestructor = true;
};
```

## Persistent
A `PersistentBase` which allows copy and assignment.
Copy, assignment and destructor bevavior is controlled by the traits class `M`.
```c++
template<typename T, typename M = NonCopyablePersistentTraits<T> > class Persistent;

template<typename T, typename M> class Persistent : public PersistentBase<T> {
 public:
 /**
  * A Persistent with no storage cell.
  */
  Persistent();

  /**
   * Construct a Persistent from a v8::Handle.
   * When the v8::Handle is non-empty, a new storage cell is created
   * pointing to the same object, and no flags are set.
   */
  template<typename S>
  Persistent(v8::Handle<S> that);

  /**
   * Construct a Persistent from a Persistent.
   * When the Persistent is non-empty, a new storage cell is created
   * pointing to the same object, and no flags are set.
   */
  Persistent(const Persistent &that);

  /**
   * The copy constructors and assignment operator create a Persistent
   * exactly as the Persistent constructor, but the Copy function from the
   * traits class is called, allowing the setting of flags based on the
   * copied Persistent.
   */
  Persistent &operator=(const Persistent &that);

  template <typename S, typename M2>
  Persistent &operator=(const Persistent<S, M2> &that);

  /**
   * The destructor will dispose the Persistent based on the
   * kResetInDestructor flags in the traits class.  Since not calling dispose
   * can result in a memory leak, it is recommended to always set this flag.
   */
  ~Persistent();
};
```

## Global
A `PersistentBase` which has move semantics.
```c++
template<typename T>
class Global : public PersistentBase<T> {
 public:
  /**
   * A Global with no storage cell.
   */
  Global();

  /**
   * Construct a Global from a v8::Handle.
   * When the v8::Handle is non-empty, a new storage cell is created
   * pointing to the same object, and no flags are set.
   */
  template<typename S>
  Global(v8::Handle<S> that);
  /**
   * Construct a Global from a PersistentBase.
   * When the Persistent is non-empty, a new storage cell is created
   * pointing to the same object, and no flags are set.
   */
  template<typename S>
  Global(const PersistentBase<S> &that);

  /**
   * Pass allows returning globals from functions, etc.
   */
  Global Pass();
};
```
