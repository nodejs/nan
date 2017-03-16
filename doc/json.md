## JSON

The _JSON_ object provides the c++ versions of the methods offered by the `JSON` object in javascript. V8 exposes these methods via the `v8::JSON` object.

 - <a href="#api_nan_json_parse"><b><code>Nan::JSON::Parse</code></b></a>
 - <a href="#api_nan_json_stringify"><b><code>Nan::JSON::Stringify</code></b></a>

Refer to the V8 JSON object in the [V8 documentation](https://v8docs.nodesource.com/node-7.4/da/d6f/classv8_1_1_j_s_o_n.html) for more information about these methods and their arguments.

<a name="api_nan_json_parse"></a>

### Nan::JSON::Parse

A simple wrapper around [`v8::JSON::Parse`](https://v8docs.nodesource.com/node-7.4/da/d6f/classv8_1_1_j_s_o_n.html#a936310d2540fb630ed37d3ee3ffe4504).

Definition:

```c++
Nan::MaybeLocal<v8::Value> Nan::JSON::Parse(v8::Local<v8::String> jsonString);
```

Use `JSON.Parse(json_string)` to parse a `v8::String` into a `v8::Value`.

Example:

```c++
v8::Local<v8::String> json_string = Nan::New("{ \"JSON\": \"object\" }").ToLocalChecked();

Nan::JSON NanJSON;
v8::Local<v8::Value> val = NanJSON.Parse(json_string).ToLocalChecked();
```

<a name="api_nan_json_stringify"></a>

### Nan::JSON::Stringify

A simple wrapper around [`v8::JSON::Stringify`](https://v8docs.nodesource.com/node-7.4/da/d6f/classv8_1_1_j_s_o_n.html#a44b255c3531489ce43f6110209138860).

Definition:

```c++
Nan::MaybeLocal<v8::String> Nan::JSON::Stringify(v8::Local<v8::Object> jsonObject, v8::Local<v8::String> gap = v8::Local<v8::String>());
```

Use `JSON.Stringify(value)` to convert a `v8::Object` into a `v8::String`.

Example:

```c++
// using `v8::Local<v8::Value> val` from the `JSON::Parse` example
v8::Local<v8::Object> obj = Nan::To<v8::Object>(val).ToLocalChecked();

Nan::JSON NanJSON;
v8::Local<v8::String> stringified = NanJSON.Stringify(obj).ToLocalChecked();
```

