#Callbacks
V8 can call back to your code through these functions.

##Types
```c++
typedef void(*FunctionCallback)(const FunctionCallbackInfo<v8::Value>&);
typedef void(*GetterCallback)
    (v8::Local<v8::String>, const PropertyCallbackInfo<v8::Value>&);
typedef void(*SetterCallback)(
    v8::Local<v8::String>,
    v8::Local<v8::Value>,
    const PropertyCallbackInfo<void>&);
typedef void(*PropertyGetterCallback)(
    v8::Local<v8::String>,
    const PropertyCallbackInfo<v8::Value>&);
typedef void(*PropertySetterCallback)(
    v8::Local<v8::String>,
    v8::Local<v8::Value>,
    const PropertyCallbackInfo<v8::Value>&);
typedef void(*PropertyEnumeratorCallback)
    (const PropertyCallbackInfo<v8::Array>&);
typedef void(*PropertyDeleterCallback)(
    v8::Local<v8::String>,
    const PropertyCallbackInfo<v8::Boolean>&);
typedef void(*PropertyQueryCallback)(
    v8::Local<v8::String>,
    const PropertyCallbackInfo<v8::Integer>&);
typedef void(*IndexGetterCallback)(
    uint32_t,
    const PropertyCallbackInfo<v8::Value>&);
typedef void(*IndexSetterCallback)(
    uint32_t,
    v8::Local<v8::Value>,
    const PropertyCallbackInfo<v8::Value>&);
typedef void(*IndexEnumeratorCallback)
    (const PropertyCallbackInfo<v8::Array>&);
typedef void(*IndexDeleterCallback)(
    uint32_t,
    const PropertyCallbackInfo<v8::Boolean>&);
typedef void(*IndexQueryCallback)(
    uint32_t,
    const PropertyCallbackInfo<v8::Integer>&);
```

##ReturnValue
Used for setting the return value in callbacks.
```c++
template<typename T>
class ReturnValue {
 public:
  // Handle setters
  template <typename S> inline void Set(const v8::Handle<S> &handle);
  template <typename S> inline void Set(const Global<S> &handle);

  // Fast primitive setters
  inline void Set(bool value);
  inline void Set(double i);
  inline void Set(int32_t i);
  inline void Set(uint32_t i);

  // Fast JS primitive setters
  inline void SetNull();
  inline void SetUndefined();
  inline void SetEmptyString();

  // Convenience getter for isolate
  inline v8::Isolate *GetIsolate() const;
};
```

##FunctionCallbackInfo
The argument information given to function call callbacks.  This
class provides access to information about the context of the call,
including the receiver, the number and values of arguments, and
the holder of the function.
```c++
template<typename T>
class FunctionCallbackInfo {
 public:
  ReturnValue<T> GetReturnValue() const;

  v8::Local<v8::Function> Callee();
  v8::Local<v8::Value> Data();
  v8::Local<v8::Object> Holder();
  bool IsConstructCall();
  int Length() const;
  v8::Local<v8::Value> operator[](int i) const;
  v8::Local<v8::Object> This() const;
  v8::Isolate *GetIsolate() const;
};
```

##PropertyCallbackInfo
The information passed to a property callback about the context of the property access.
This class provides access to information about the context of the access, including the
receiver and the holder of the property.
```c++
template<typename T>
class PropertyCallbackInfo : public PropertyCallbackInfoBase<T> {
 public:
  ReturnValue<T> GetReturnValue() const;

  v8::Isolate* GetIsolate() const;
  v8::Local<v8::Value> Data() const;
  v8::Local<v8::Object> This() const;
  v8::Local<v8::Object> Holder() const;
};
```
