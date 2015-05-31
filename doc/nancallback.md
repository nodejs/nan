#Callback
`Callback` makes it easier to use `v8::Function` handles as callbacks.

##Callback
A class that wraps a `v8::Function` handle, protecting it from garbage collection.
```c++
class Callback {
 public:
  Callback();

  explicit Callback(const v8::Handle<v8::Function> &fn);

  ~Callback();

  bool operator==(const Callback &other) const;

  bool operator!=(const Callback &other) const;

  NAN_INLINE
  v8::Local<v8::Function> operator*() const;

  v8::Local<v8::Value> operator()(
      v8::Handle<v8::Object> target
    , int argc = 0
    , v8::Handle<v8::Value> argv[] = 0) const;

  v8::Local<v8::Value> operator()(
      int argc = 0
    , v8::Handle<v8::Value> argv[] = 0) const;

  void SetFunction(const v8::Handle<v8::Function> &fn);

  v8::Local<v8::Function> GetFunction() const;

  bool IsEmpty() const;

  v8::Local<v8::Value>
  Call(v8::Handle<v8::Object> target
     , int argc
     , v8::Handle<v8::Value> argv[]) const;

  v8::Local<v8::Value>
  Call(int argc, v8::Handle<v8::Value> argv[]) const;
};
```

###Example
```c++
v8::Local<v8::Function> function;
Callback callback(function);
callback->Call(0, 0);
```
