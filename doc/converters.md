#Converters
These functions convert `v8::Value`s to other `v8::Value` types and native types.
Since type conversion is not guaranteed to succeed, they return `Maybe` types.

## To
Converts a `v8::Handle<v8::Value>` to a different subtype of `v8::Value` or to a native
data type. Returns a `MaybeLocal<>` or a `Maybe<>` accordingly.

```c++
MaybeLocal<v8::Boolean> To<>(v8::Handle<v8::Value> val);
MaybeLocal<v8::Int32>   To<>(v8::Handle<v8::Value> val);
MaybeLocal<v8::Integer> To<>(v8::Handle<v8::Value> val);
MaybeLocal<v8::Object>  To<>(v8::Handle<v8::Value> val);
MaybeLocal<v8::Number>  To<>(v8::Handle<v8::Value> val);
MaybeLocal<v8::String>  To<>(v8::Handle<v8::Value> val);
MaybeLocal<v8::Uint32>  To<>(v8::Handle<v8::Value> val);

Maybe<bool>             To<>(v8::Handle<v8::Value> val);
Maybe<double>           To<>(v8::Handle<v8::Value> val);
Maybe<int32_t>          To<>(v8::Handle<v8::Value> val);
Maybe<int64_t>          To<>(v8::Handle<v8::Value> val);
Maybe<uint32_t>         To<>(v8::Handle<v8::Value> val);
```

###Example
```c++
v8::Local<v8::Value> val;
MaybeLocal<v8::String> = To<v8::String>(val);
Maybe<double> = To<double>(val);
```
