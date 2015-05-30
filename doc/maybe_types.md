#Maybe Types
The `MaybeLocal` and `Maybe` types are monads that encapsulate possibly
empty `v8::Local` handles.

##MaybeLocal
A `MaybeLocal<>` is a wrapper around `v8::Local<>` that enforces a check whether
the `v8::Local<>` is empty before it can be used.

If an API method returns a `MaybeLocal<>`, the API method can potentially fail
either because an exception is thrown, or because an exception is pending,
e.g. because a previous API call threw an exception that hasn't been caught
yet, or because a `v8::TerminateExecution` exception was thrown. In that case, an
empty `MaybeLocal` is returned.
```c++
template<typename T>
class MaybeLocal {
 public:
  MaybeLocal();

  template<typename S>
  MaybeLocal(v8::Local<S> that);

  bool IsEmpty() const;

  template<typename S>
  bool ToLocal(v8::Local<S> *out);

  // Will crash if the MaybeLocal<> is empty.
  v8::Local<T> ToLocalChecked();

  template<typename S>
  v8::Local<S> FromMaybe(v8::Local<S> default_value) const;
};
```

##Maybe
A simple `Maybe` type, representing an object which may or may not have a
value, see https://hackage.haskell.org/package/base/docs/Data-Maybe.html.

If an API method returns a `Maybe<>`, the API method can potentially fail
either because an exception is thrown, or because an exception is pending,
e.g. because a previous API call threw an exception that hasn't been caught
yet, or because a `v8::TerminateExecution` exception was thrown. In that case,
a "Nothing" value is returned.
```c++
template<typename T>
class Maybe {
 public:
  bool IsNothing() const;
  bool IsJust() const;

  // Will crash if the Maybe<> is nothing.
  T FromJust();

  T FromMaybe(const T& default_value);

  bool operator==(const Maybe &other);

  bool operator!=(const Maybe &other);
};
```

## Nothing
Construct an empty `Maybe` type representing "Nothing".
```c++
template<typename T>
Maybe<T> Nothing();
```

## Just
Construct a `Maybe` type representing "Just" a value.
```c++
template<typename T>
Maybe<T> Just(const T &t);
```
