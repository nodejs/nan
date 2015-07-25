Across the versions of V8 supported by NAN, JavaScript-accessible method signatures vary widely, NAN fully abstracts method declaration and provides you with an interface that is similar to the most recent V8 API but is backward-compatible with older versions that still use the now-deceased `v8::Argument` type.

* **Method argument types**
 - <a href="#api_nan_function_callback_info"><b><code>Nan::FunctionCallbackInfo</code></b></a>
 - <a href="#api_nan_property_callback_info"><b><code>Nan::PropertyCallbackInfo</code></b></a>
* **Method declarations**
 - <a href="#api_nan_method"><b>Method declaration</b></a>
 - <a href="#api_nan_getter"><b>Getter declaration</b></a>
 - <a href="#api_nan_setter"><b>Setter declaration</b></a>
 - <a href="#api_nan_property_getter"><b>Property getter declaration</b></a>
 - <a href="#api_nan_property_setter"><b>Property setter declaration</b></a>
 - <a href="#api_nan_property_enumerator"><b>Property enumerator declaration</b></a>
 - <a href="#api_nan_property_deleter"><b>Property deleter declaration</b></a>
 - <a href="#api_nan_property_query"><b>Property query declaration</b></a>
 - <a href="#api_nan_index_getter"><b>Index getter declaration</b></a>
 - <a href="#api_nan_index_setter"><b>Index setter declaration</b></a>
 - <a href="#api_nan_index_enumerator"><b>Index enumerator declaration</b></a>
 - <a href="#api_nan_index_deleter"><b>Index deleter declaration</b></a>
 - <a href="#api_nan_index_query"><b>Index query declaration</b></a>

<a name="api_nan_function_callback_info"></a>
### Nan::FunctionCallbackInfo

`Nan::FunctionCallbackInfo` should be used in place of [`v8::FunctionCallbackInfo`](https://v8docs.nodesource.com/io.js-3.0/dd/d0d/classv8_1_1_function_callback_info.html), even with older versions of Node where `v8::FunctionCallbackInfo` does not exist.

The following methods are available:

* `Nan::ReturnValue<T> GetReturnValue()`
* `v8::Local<v8::Function> Callee()`
* `v8::Local<v8::Value> Data()`
* `v8::Local<v8::Object> Holder()`
* `bool IsConstructCall()`
* `int Length()`
* `v8::Local<v8::Value> operator[](int i)`
* `v8::Local<v8::Object> This()`
* `v8::Isolate *GetIsolate()`

See the [`v8::FunctionCallbackInfo](https://v8docs.nodesource.com/io.js-3.0/dd/d0d/classv8_1_1_function_callback_info.html) documentation for usage details on these. Note that `Nan::ReturnValue` is also a mirror of the equivalent `v8::ReturnValue` type for similar reasons.

<a name="api_nan_property_callback_info"></a>
### Nan::PropertyCallbackInfo

`Nan::PropertyCallbackInfo` should be used in place of [`v8::PropertyCallbackInfo](https://v8docs.nodesource.com/io.js-3.0/d7/dc5/classv8_1_1_property_callback_info.html), even with older versions of Node where `v8::PropertyCallbackInfo` does not exist.

The following methods are available:

* `v8::Isolate* GetIsolate()`
* `v8::Local<v8::Value> Data()`
* `v8::Local<v8::Object> This()`
* `v8::Local<v8::Object> Holder()`
* `Nan::ReturnValue<T> GetReturnValue()`

See the [`v8::PropertyCallbackInfo](https://v8docs.nodesource.com/io.js-3.0/d7/dc5/classv8_1_1_property_callback_info.html) documentation for usage details on these. Note that `Nan::ReturnValue` is also a mirror of the equivalent `v8::ReturnValue` type for similar reasons.

<a name="api_nan_method"></a>
### Method declaration

JavaScript-accessible methods should be declared with the following signature:

```c++
void MethodName(const Nan::FunctionCallbackInfo<v8::Value>& info);
```

You do not need to declare a new `HandleScope` within a method as one is implicitly created for you.

**Example usage**

```c++
// .h:
class Foo : public Nan::ObjectWrap {
  ...

  static void Bar(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Baz(const Nan::FunctionCallbackInfo<v8::Value>& info);
}


// .cc:
void Foo::Bar(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  ...
}

void Foo::Baz(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  ...
}
```

A helper macro `NAN_METHOD(methodname)` exists, compatible with NAN v1 method declarations.

**Example usage with `NAN_METHOD(methodname)`**

```c++
// .h:
class Foo : public Nan::ObjectWrap {
  ...

  static NAN_METHOD(Bar);
  static NAN_METHOD(Baz);
}


// .cc:
NAN_METHOD(Foo::Bar) {
  ...
}

NAN_METHOD(Foo::Baz) {
  ...
}
```


<a name="api_nan_getter"></a>
### Getter declaration

JavaScript-accessible getters should be declared with the following signature:

```c++
void GetterName(v8::Local<v8::String> property,
                const Nan::PropertyCallbackInfo<v8::Value>& info);
```

You do not need to declare a new `HandleScope` within a getter as one is implicitly created for you.

A helper macro `NAN_GETTER(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_setter"></a>
### Setter declaration

JavaScript-accessible setters should be declared with the following signature:

```c++
void SetterName(v8::Local<v8::String> property,
                v8::Local<v8::Value> value,
                const Nan::PropertyCallbackInfo<v8::Value>& info);
```

You do not need to declare a new `HandleScope` within a setter as one is implicitly created for you.

A helper macro `NAN_SETTER(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_property_getter"></a>
### Property getter declaration

JavaScript-accessible property getters should be declared with the following signature:

```c++
void PropertyGetterName(v8::Local<v8::String> property,
                        const Nan::PropertyCallbackInfo<v8::Value>& info);
```

You do not need to declare a new `HandleScope` within a property getter as one is implicitly created for you.

A helper macro `NAN_PROPERTY_GETTER(methodname)` exists, compatible with NAN v1 method declarations.


<a name="api_nan_property_setter"></a>
### Property setter declaration

JavaScript-accessible property setters should be declared with the following signature:

```c++
void PropertySetterName(v8::Local<v8::String> property,
                        v8::Local<v8::Value> value,
                        const Nan::PropertyCallbackInfo<v8::Value>& info);
```

You do not need to declare a new `HandleScope` within a property setter as one is implicitly created for you.

A helper macro `NAN_PROPERTY_SETTER(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_property_enumerator"></a>
### Property enumerator declaration

JavaScript-accessible property enumerators should be declared with the following signature:

```c++
void PropertyEnumeratorName(const Nan::PropertyCallbackInfo<v8::Array>& info);
```

You do not need to declare a new `HandleScope` within a property enumerator as one is implicitly created for you.

A helper macro `NAN_PROPERTY_ENUMERATOR(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_property_deleter"></a>
### Property deleter declaration

JavaScript-accessible property deleters should be declared with the following signature:

```c++
void PropertyDeleterName(v8::Local<v8::String> property,
                         const Nan::PropertyCallbackInfo<v8::Boolean>& info);
```

You do not need to declare a new `HandleScope` within a property deleter as one is implicitly created for you.

A helper macro `NAN_PROPERTY_DELETER(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_property_query"></a>
### Property query declaration

JavaScript-accessible property query methods should be declared with the following signature:

```c++
void PropertyQueryName(v8::Local<v8::String> property,
                       const Nan::PropertyCallbackInfo<v8::Integer>& info);
```

You do not need to declare a new `HandleScope` within a property query method as one is implicitly created for you.

A helper macro `NAN_PROPERTY_QUERY(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_index_getter"></a>
### Index getter declaration

JavaScript-accessible index getter methods should be declared with the following signature:

```c++
void IndexGetterName(uint32_t index, const PropertyCallbackInfo<v8::Value>& info);
```

You do not need to declare a new `HandleScope` within a index getter as one is implicitly created for you.

A helper macro `NAN_INDEX_GETTER(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_index_setter"></a>
### Index setter declaration

JavaScript-accessible index setter methods should be declared with the following signature:

```c++
void IndexSetterName(uint32_t index,
                     v8::Local<v8::Value> value,
                     const PropertyCallbackInfo<v8::Value>& info);
```

You do not need to declare a new `HandleScope` within a index setter as one is implicitly created for you.

A helper macro `NAN_INDEX_SETTER(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_index_enumerator"></a>
### Index enumerator declaration

JavaScript-accessible index enumerator methods should be declared with the following signature:

```c++
void IndexEnumeratorName(const PropertyCallbackInfo<v8::Array>& info);
```

You do not need to declare a new `HandleScope` within a index enumerator as one is implicitly created for you.

A helper macro `NAN_INDEX_ENUMERATOR(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_index_deleter"></a>
### Index deleter declaration

JavaScript-accessible index deleter methods should be declared with the following signature:

```c++
void IndexDeleterName(uint32_t index, const PropertyCallbackInfo<v8::Boolean>& info);
```

You do not need to declare a new `HandleScope` within a index deleter as one is implicitly created for you.

A helper macro `NAN_INDEX_DELETER(methodname)` exists, compatible with NAN v1 method declarations.

<a name="api_nan_index_query"></a>
### Index query declaration

JavaScript-accessible index query methods should be declared with the following signature:

```c++
void IndexQueryName(uint32_t index, const PropertyCallbackInfo<v8::Integer>& info);
```

You do not need to declare a new `HandleScope` within a index query method as one is implicitly created for you.

A helper macro `NAN_INDEX_QUERY(methodname)` exists, compatible with NAN v1 method declarations.

