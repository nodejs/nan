## Maybe Types

The `Nan::MaybeLocal` and `Nan::Maybe` types are monads that encapsulate `v8::Local` handles that _may be empty_.

 - <a href="#api_nan_maybe_local"><b><code>Nan::MaybeLocal</code></b></a>
 - <a href="#api_nan_maybe"><b><code>Nan::Maybe</code></b></a>
 - <a href="#api_nan_nothing"><b><code>Nan::Nothing</code></b></a>
 - <a href="#api_nan_just"><b><code>Nan::Just</code></b></a>

<a name="api_nan_maybe_local"></a>
### Nan::MaybeLocal

A `Nan::MaybeLocal<T>` is a wrapper around [`v8::Local<T>`](https://v8docs.nodesource.com/io.js-3.0/de/deb/classv8_1_1_local.html) that enforces a check that determines whether the `v8::Local<T>` is empty before it can be used.

If an API method returns a `Nan::MaybeLocal`, the API method can potentially fail either because an exception is thrown, or because an exception is pending, e.g. because a previous API call threw an exception that hasn't been caught yet, or because a `v8::TerminateExecution` exception was thrown. In that case, an empty `Nan::MaybeLocal` is returned.

Definition:

```c++
template<typename T> class Nan::MaybeLocal {
 public:
  MaybeLocal();

  template<typename S> MaybeLocal(v8::Local<S> that);

  bool IsEmpty() const;

  template<typename S> bool ToLocal(v8::Local<S> *out);

  // Will crash if the MaybeLocal<> is empty.
  v8::Local<T> ToLocalChecked();

  template<typename S> v8::Local<S> FromMaybe(v8::Local<S> default_value) const;
};
```

See the documentation for [`v8::MaybeLocal`](https://v8docs.nodesource.com/io.js-3.0/d8/d7d/classv8_1_1_maybe_local.html) for further details.

<a name="api_nan_maybe"></a>
## Nan::Maybe

A simple `Nan::Maybe` type, representing an object which may or may not have a value, see https://hackage.haskell.org/package/base/docs/Data-Maybe.html.

If an API method returns a `Nan::Maybe<>`, the API method can potentially fail either because an exception is thrown, or because an exception is pending, e.g. because a previous API call threw an exception that hasn't been caught yet, or because a `v8::TerminateExecution` exception was thrown. In that case, a "Nothing" value is returned.

Definition:

```c++
template<typename T> class Nan::Maybe {
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

See the documentation for [`v8::Maybe`](https://v8docs.nodesource.com/io.js-3.0/d9/d4b/classv8_1_1_maybe.html) for further details.

<a name="api_nan_nothing"></a>
## Nan::Nothing

Construct an empty `Nan::Maybe` type representing _nothing_.

```c++
template<typename T> Nan::Maybe<T> Nan::Nothing();
```

<a name="api_nan_just"></a>
## Nan::Just

Construct a `Nan::Maybe` type representing _just_ a value.

```c++
template<typename T> Nan::Maybe<T> Nan::Just(const T &t);
```
