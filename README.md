Native Abstractions for Node.js
===============================

**A header file filled with macro and utility goodness for making add-on development for Node.js easier across versions 0.8, 0.10 and 0.12 as well as io.js.**

***Current version: 1.8.4***

*(See [CHANGELOG.md](https://github.com/nodejs/nan/blob/master/CHANGELOG.md) for complete ChangeLog)*

[![NPM](https://nodei.co/npm/nan.png?downloads=true&downloadRank=true)](https://nodei.co/npm/nan/) [![NPM](https://nodei.co/npm-dl/nan.png?months=6&height=3)](https://nodei.co/npm/nan/)

[![Build Status](https://api.travis-ci.org/nodejs/nan.svg?branch=master)](http://travis-ci.org/nodejs/nan)
[![Build status](https://ci.appveyor.com/api/projects/status/kh73pbm9dsju7fgh)](https://ci.appveyor.com/project/RodVagg/nan)

Thanks to the crazy changes in V8 (and some in Node core), keeping native addons compiling happily across versions, particularly 0.10 to 0.12, is a minor nightmare. The goal of this project is to store all logic necessary to develop native Node.js addons without having to inspect `NODE_MODULE_VERSION` and get yourself into a macro-tangle.

This project also contains some helper utilities that make addon development a bit more pleasant.

 * **[News & Updates](#news)**
 * **[Usage](#usage)**
 * **[Example](#example)**
 * **[API](#api)**
 * **[Tests](#tests)**
 * **[Porting](#porting)**
 * **[Governance & Contributing](#governance)**

<a name="news"></a>
## News & Updates


<a name="usage"></a>
## Usage

Simply add **NAN** as a dependency in the *package.json* of your Node addon:

``` bash
$ npm install --save nan
```

Pull in the path to **NAN** in your *binding.gyp* so that you can use `#include <nan.h>` in your *.cpp* files:

``` python
"include_dirs" : [
    "<!(node -e \"require('nan')\")"
]
```

This works like a `-I<path-to-NAN>` when compiling your addon.

<a name="example"></a>
## Example

Just getting started with Nan? Refer to a [quick-start **Nan** Boilerplate](https://github.com/fcanas/node-native-boilerplate) for a ready-to-go project that utilizes basic Nan functionality.

See **[LevelDOWN](https://github.com/rvagg/node-leveldown/pull/48)** for a full example of **NAN** in use.

For a simpler example, see the **[async pi estimation example](https://github.com/nodejs/nan/tree/master/examples/async_pi_estimate)** in the examples directory for full code and an explanation of what this Monte Carlo Pi estimation example does. Below are just some parts of the full example that illustrate the use of **NAN**.

For another example, see **[nan-example-eol](https://github.com/CodeCharmLtd/nan-example-eol)**. It shows newline detection implemented as a native addon.

Compare to the current 0.10 version of this example, found in the [node-addon-examples](https://github.com/iojs/node-addon-examples/tree/master/9_async_work) repository and also a 0.11 version of the same found [here](https://github.com/kkoopa/node-addon-examples/tree/5c01f58fc993377a567812597e54a83af69686d7/9_async_work).

Note that there is no embedded version sniffing going on here and also the async work is made much simpler, see below for details on the `NanAsyncWorker` class.

```c++
// addon.cc
#include <node.h>
#include <nan.h>
// ...

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;

void InitAll(Handle<Object> exports) {
  exports->Set(NanNew<String>("calculateSync"),
    NanNew<FunctionTemplate>(CalculateSync)->GetFunction());

  exports->Set(NanNew<String>("calculateAsync"),
    NanNew<FunctionTemplate>(CalculateAsync)->GetFunction());
}

NODE_MODULE(addon, InitAll)
```

```c++
// sync.h
#include <node.h>
#include <nan.h>

NAN_METHOD(CalculateSync);
```

```c++
// sync.cc
#include <node.h>
#include <nan.h>
#include "./sync.h"
// ...

using v8::Number;

// Simple synchronous access to the `Estimate()` function
NAN_METHOD(CalculateSync) {
  // expect a number as the first argument
  int points = args[0]->Uint32Value();
  double est = Estimate(points);

  NanReturnValue(NanNew<Number>(est));
}
```

```c++
// async.h
#include <node.h>
#include <nan.h>

NAN_METHOD(CalculateAsync);
```

```c++
// async.cc
#include <node.h>
#include <nan.h>
#include "./async.h"

// ...

using v8::Function;
using v8::Local;
using v8::Null;
using v8::Number;
using v8::Value;

class PiWorker : public NanAsyncWorker {
 public:
  PiWorker(NanCallback *callback, int points)
    : NanAsyncWorker(callback), points(points) {}
  ~PiWorker() {}

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    estimate = Estimate(points);
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    NanScope();

    Local<Value> argv[] = {
        NanNull()
      , NanNew<Number>(estimate)
    };

    callback->Call(2, argv);
  };

 private:
  int points;
  double estimate;
};

// Asynchronous access to the `Estimate()` function
NAN_METHOD(CalculateAsync) {
  int points = args[0]->Uint32Value();
  NanCallback *callback = new NanCallback(args[1].As<Function>());

  NanAsyncQueueWorker(new PiWorker(callback, points));
  NanReturnUndefined();
}
```

<a name="api"></a>
## API

<!-- START API -->

API docs in here

<!-- END API -->



```
 * <a href="#api_nan_weak_callback"><b><code>NAN_WEAK_CALLBACK(callbackname)</code></b></a>
 * <a href="#api_nan_deprecated"><b><code>NAN_DEPRECATED</code></b></a>
 * <a href="#api_nan_inline"><b><code>NAN_INLINE</code></b></a>
 * <a href="#api_nan_new"><b><code>NanNew</code></b></a>
 * <a href="#api_nan_undefined"><b><code>NanUndefined</code></b></a>
 * <a href="#api_nan_null"><b><code>NanNull</code></b></a>
 * <a href="#api_nan_true"><b><code>NanTrue</code></b></a>
 * <a href="#api_nan_false"><b><code>NanFalse</code></b></a>
 * <a href="#api_nan_scope"><b><code>NanScope</code></b></a>
 * <a href="#api_nan_escapable_scope"><b><code>NanEscapableScope</code></b></a>
 * <a href="#api_nan_escape_scope"><b><code>NanEscapeScope</code></b></a>
 * <a href="#api_nan_object_wrap_handle"><b><code>NanObjectWrapHandle</code></b></a>
 * <a href="#api_nan_ascii_string"><b><code>NanAsciiString</code></b></a>
 * <a href="#api_nan_utf8_string"><b><code>NanUtf8String</code></b></a>
 * <a href="#api_nan_ucs2_string"><b><code>NanUcs2String</code></b></a>
 * <a href="#api_nan_error"><b><code>NanError</code></b>, <b><code>NanRangeError</code></b>, <b><code>NanReferenceError</code></b>, <b><code>NanSyntaxError</code></b>, <b><code>NanTypeError</code></b></a>
 * <a href="#api_nan_throw_error"><b><code>NanThrowError</code></b>, <b><code>NanThrowRangeError</code></b>, <b><code>NanThrowReferenceError</code></b>, <b><code>NanThrowSyntaxError</code></b>, <b><code>NanThrowTypeError</code></b>, <b><code>NanThrowError(Handle<Value>)</code></b>, <b><code>NanThrowError(Handle<Value>, int)</code></b></a>
 * <a href="#api_nan_new_buffer_handle"><b><code>NanNewBufferHandle(char *, size_t, FreeCallback, void *)</code></b>, <b><code>NanNewBufferHandle(char *, uint32_t)</code></b>, <b><code>NanNewBufferHandle(uint32_t)</code></b></a>
 * <a href="#api_nan_buffer_use"><b><code>NanBufferUse(char *, uint32_t)</code></b></a>
 * <a href="#api_nan_get_current_context"><b><code>NanGetCurrentContext</code></b></a>
 * <a href="#api_nan_has_instance"><b><code>NanHasInstance</code></b></a>
 * <a href="#api_nan_dispose_persistent"><b><code>NanDisposePersistent</code></b></a>
 * <a href="#api_nan_assign_persistent"><b><code>NanAssignPersistent</code></b></a>
 * <a href="#api_nan_make_weak_persistent"><b><code>NanMakeWeakPersistent</code></b></a>
 * <a href="#api_nan_make_callback"><b><code>NanMakeCallback</code></b></a>
 * <a href="#api_nan_fatal_exception"><b><code>NanFatalException</code></b></a>
 * <a href="#api_nan_errno_exception"><b><code>NanErrnoException</code></b></a>
 * <a href="#api_nan_get_isolate_data"><b><code>NanGetIsolateData</code></b></a>
 * <a href="#api_nan_set_isolate_data"><b><code>NanSetIsolateData</code></b></a>
 * <a href="#api_nan_encode"><b><code>NanEncode</code></b></a>
 * <a href="#api_nan_decode_bytes"><b><code>NanDecodeBytes</code></b></a>
 * <a href="#api_nan_decode_write"><b><code>NanDecodeWrite</code></b></a>
 * <a href="#api_nan_compile_script"><b><code>NanCompileScript</code></b></a>
 * <a href="#api_nan_run_script"><b><code>NanRunScript</code></b></a>
 * <a href="#api_nan_callback"><b><code>NanCallback</code></b></a>
 * <a href="#api_nan_async_worker"><b><code>NanAsyncWorker</code></b></a>
 * <a href="#api_nan_async_queue_worker"><b><code>NanAsyncQueueWorker</code></b></a>

<a name="api_nan_weak_callback"></a>
### NAN_WEAK_CALLBACK(callbackname)

Use `NAN_WEAK_CALLBACK` to define your V8 WeakReference callbacks. There is an argument object `const _NanWeakCallbackData<T, P> &data` allowing access to the weak object and the supplied parameter through its `GetValue` and `GetParameter` methods. You can even access the weak callback info object through the `GetCallbackInfo()`method, but you probably should not. `Revive()` keeps the weak object alive until the next GC round.

```c++
NAN_WEAK_CALLBACK(weakCallback) {
  int *parameter = data.GetParameter();
  NanMakeCallback(NanGetCurrentContext()->Global(), data.GetValue(), 0, NULL);
  if ((*parameter)++ == 0) {
    data.Revive();
  } else {
    delete parameter;
  }
}

<a name="api_nan_deprecated"></a>
### NAN_DEPRECATED
Declares a function as deprecated.

```c++
static NAN_DEPRECATED NAN_METHOD(foo) {
  ...
}
```

<a name="api_nan_inline"></a>
### NAN_INLINE
Inlines a function.

```c++
NAN_INLINE int foo(int bar) {
  ...
}
```

<a name="api_nan_new"></a>
### Local&lt;T&gt; NanNew&lt;T&gt;( ... )

Use `NanNew` to construct almost all v8 objects (bound `Script`s are constructed with <a href="#api_nan_compile_script">`NanCompileScript(Handle)`</a>) and make new local handles.

```c++
Local<String> s = NanNew<String>("value");

...

Persistent<Object> o;

...

Local<Object> lo = NanNew(o);

```

<a name="api_nan_undefined"></a>
### Local&lt;Primitive&gt; NanUndefined()

Use instead of `Undefined()`

<a name="api_nan_null"></a>
### Local&lt;Primitive&gt; NanNull()

Use instead of `Null()`

<a name="api_nan_true"></a>
### Local&lt;Boolean&gt; NanTrue()

Use instead of `True()`

<a name="api_nan_false"></a>
### Local&lt;Boolean&gt; NanFalse()

Use instead of `False()`

<a name="api_nan_scope"></a>
### NanScope()

The introduction of `isolate` references for many V8 calls in Node 0.11 makes `NanScope()` necessary, use it in place of `HandleScope scope` when you do not wish to return handles (`Handle` or `Local`) to the surrounding scope (or in functions directly exposed to V8, as they do not return values in the normal sense). V8-exposed methods (NAN_METHOD, etc.) have implicit handle scopes and do not need extra scopes:

```c++
NAN_METHOD(Foo::Bar) {
  NanReturnValue(NanNew<String>("FooBar!"));
}
```

This method is not directly exposed to V8, nor does it return a handle, so it uses an unescapable scope:

```c++
bool Foo::Bar() {
  NanScope();

  Local<Boolean> val = NanFalse();
  ...
  return val->Value();
}
```

<a name="api_nan_escapable_scope"></a>
### NanEscapableScope()

The separation of handle scopes into escapable and inescapable scopes makes `NanEscapableScope()` necessary, use it in place of `HandleScope scope` when you later wish to return a handle (`Handle` or `Local`) from the scope, this is for internal functions not directly exposed to V8:

```c++
Handle<String> Foo::Bar() {
  NanEscapableScope();

  return NanEscapeScope(NanNew<String>("FooBar!"));
}
```

<a name="api_nan_escape_scope"></a>
### Local&lt;T&gt; NanEscapeScope(Handle&lt;T&gt; value);
Use together with `NanEscapableScope` to escape the scope. Corresponds to `HandleScope::Close` or `EscapableHandleScope::Escape`.


<a name="api_nan_object_wrap_handle"></a>
### Local&lt;Object&gt; NanObjectWrapHandle(const node::ObjectWrap &amp;obj)

When you want to fetch the V8 object handle from a native object you've wrapped with Node's `ObjectWrap`, you should use `NanObjectWrapHandle`:

```c++
NanObjectWrapHandle(iterator)->Get(NanNew<String>("end"))
```

<a name="api_nan_ascii_string"></a>
### NanAsciiString

Contrary to the name, this is not actually an ASCII string, it is a one-byte string with no particular encoding. Do not use unless you actually need this incorrect legacy behavior. Consider fixing your broken code instead. If you actually have a proper ASCII-string, use UTF-8, which is a proper superset of ASCII.
Convert a `String` to zero-terminated, sort-of Ascii-encoded `char *`. The underlying buffer is freed when the owner object goes out of scope, so make a copy or heap allocation if you need it to stick around.

```c++
NAN_METHOD(foo) {
  NanReturnValue(NanNew(*NanAsciiString(arg[0])));
}
```

####*WRONG*:
the buffer `str` points to has been freed when `baz` was destroyed:
```c++
static char *str;

NAN_METHOD(bar) {
  NanAsciiString baz(arg[0]);

  str = *baz;
  NanReturnUndefined(); // baz goes out of scope, freeing str
}

...

printf(str); // use-after-free error
```

####*RIGHT*:
```c++
static NanAsciiString *str;

NAN_METHOD(bar) {
  str = new NanAsciiString(arg[0]);
  NanReturnUndefined();
}

...

printf(**str);
```

<a name="api_nan_utf8_string"></a>
### NanUtf8String

Equivalent to `String::Utf8Value`, it only exists for the sake of completeness.
Convert a `String` to zero-terminated, Utf8-encoded `char *`. The underlying buffer is freed when the owner object goes out of scope, so make a copy or heap allocation if you need it to stick around.

```c++
NAN_METHOD(foo) {
  NanReturnValue(NanNew(*NanUtf8String(arg[0])));
}
```

####*WRONG*:
the buffer `str` points to has been freed when `baz` was destroyed:
```c++
static char *str;

NAN_METHOD(bar) {
  NanUtf8String baz(arg[0]);

  str = *baz;
  NanReturnUndefined(); // baz goes out of scope, freeing str
}

...

printf(str); // use-after-free error
```

####*RIGHT*:
```c++
static NanUtf8String *str;

NAN_METHOD(bar) {
  str = new NanUtf8String(arg[0]);
  NanReturnUndefined();
}

...

printf(**str);
```


<a name="api_nan_ucs2_string"></a>
### NanUcs2String

Equivalent to `String::Value`, it only exists for the sake of completeness.
Convert a `String` to zero-terminated, Ucs2-encoded `uint16_t *`. The underlying buffer is freed when the owner object goes out of scope, so make a copy or heap allocation if you need it to stick around.

```c++
NAN_METHOD(foo) {
  NanReturnValue(NanNew(*NanUcs2String(arg[0])));
}
```

####*WRONG*:
the buffer `str` points to has been freed when `baz` was destroyed:
```c++
static char *str;

NAN_METHOD(bar) {
  NanUcs2String baz(arg[0]);

  str = *baz;
  NanReturnUndefined(); // baz goes out of scope, freeing str
}

...

printf(str); // use-after-free error
```

####*RIGHT*:
```c++
static NanUcs2String *str;

NAN_METHOD(bar) {
  str = new NanUcs2String(arg[0]);
  NanReturnUndefined();
}

...

printf(**str);
```

<a name="api_nan_error"></a>
### NanError(message), NanRangeError(message), NanReferenceError(message), NanSyntaxError(message), NanTypeError(message)

For making `Error`, `RangeError`, `ReferenceError`, `SyntaxError` and `TypeError` objects.

```c++
Local<Value> res = NanError("you must supply a callback argument");
```

<a name="api_nan_throw_error"></a>
### NanThrowError(message), NanThrowRangeError(message), NanThrowReferenceError(message), NanThrowSyntaxError(message), NanThrowTypeError(message), NanThrowError(Local&lt;Value&gt;), NanThrowError(Local&lt;Value&gt;, int)

For throwing `Error`, `RangeError`, `ReferenceError`, `SyntaxError` and `TypeError` objects.

```c++
NanThrowError("you must supply a callback argument");
```

Can also handle any custom object you may want to throw. If used with the error code argument, it will add the supplied error code to the error object as a property called `code`.

<a name="api_nan_new_buffer_handle"></a>
### Local&lt;Object&gt; NanNewBufferHandle(char *, uint32_t), Local&lt;Object&gt; NanNewBufferHandle(uint32_t)

The `Buffer` API has changed a little in Node 0.11, this helper provides consistent access to `Buffer` creation:

```c++
NanNewBufferHandle((char*)value.data(), value.size());
```

Can also be used to initialize a `Buffer` with just a `size` argument.

Can also be supplied with a `NanFreeCallback` and a hint for the garbage collector.

<a name="api_nan_buffer_use"></a>
### Local&lt;Object&gt; NanBufferUse(char*, uint32_t)

`Buffer::New(char*, uint32_t)` prior to 0.11 would make a copy of the data.
While it was possible to get around this, it required a shim by passing a
callback. So the new API `Buffer::Use(char*, uint32_t)` was introduced to remove
needing to use this shim.

`NanBufferUse` uses the `char*` passed as the backing data, and will free the
memory automatically when the weak callback is called. Keep this in mind, as
careless use can lead to "double free or corruption" and other cryptic failures.

<a name="api_nan_has_instance"></a>
### bool NanHasInstance(Persistent&lt;FunctionTemplate&gt;&, Handle&lt;Value&gt;)

Can be used to check the type of an object to determine it is of a particular class you have already defined and have a `Persistent<FunctionTemplate>` handle for.

<a name="api_nan_get_current_context"></a>
### Local&lt;Context&gt; NanGetCurrentContext()

Gets the current context.

```c++
Local<Context> ctx = NanGetCurrentContext();
```

<a name="api_nan_dispose_persistent"></a>
### void NanDisposePersistent(Persistent&lt;T&gt; &)

Use `NanDisposePersistent` to dispose a `Persistent` handle.

```c++
NanDisposePersistent(persistentHandle);
```

<a name="api_nan_assign_persistent"></a>
### NanAssignPersistent(handle, object)

Use `NanAssignPersistent` to assign a non-`Persistent` handle to a `Persistent` one. You can no longer just declare a `Persistent` handle and assign directly to it later, you have to `Reset` it in Node 0.11, so this makes it easier.

In general it is now better to place anything you want to protect from V8's garbage collector as properties of a generic `Object` and then assign that to a `Persistent`. This works in older versions of Node also if you use `NanAssignPersistent`:

```c++
Persistent<Object> persistentHandle;

...

Local<Object> obj = NanNew<Object>();
obj->Set(NanNew<String>("key"), keyHandle); // where keyHandle might be a Local<String>
NanAssignPersistent(persistentHandle, obj)
```

<a name="api_nan_make_weak_persistent"></a>
### _NanWeakCallbackInfo&lt;T, P&gt;* NanMakeWeakPersistent(Handle&lt;T&gt;, P*, _NanWeakCallbackInfo&lt;T, P&gt;::Callback)

Creates a weak persistent handle with the supplied parameter and `NAN_WEAK_CALLBACK`.

```c++
NAN_WEAK_CALLBACK(weakCallback) {

...

}

Local<Function> func;

...

int *parameter = new int(0);
NanMakeWeakPersistent(func, parameter, &weakCallback);
```

<a name="api_nan_make_callback"></a>
### NanMakeCallback(target, func, argc, argv)

Use instead of `node::MakeCallback` to call javascript functions. This (or `NanCallback`) is the only proper way of calling functions. You must _*never, ever*_ directly use `Function::Call`, it will lead to run-time failures.

<a name="api_nan_fatal_exception"></a>
### NanFatalException(const v8::TryCatch &amp;)

Replaces `node::FatalException`.

<a name="api_nan_errno_exception"></a>
### NanErrnoException(int errorno, const char *syscall = NULL, const char *message = NULL, const char *path = NULL)

Replaces `node::ErrnoException`.

<a name="api_nan_get_isolate_data"></a>
### NanGetIsolateData(v8::Isolate *)

Replaces `v8::Isolate::GetData`.

<a name="api_nan_set_isolate_data"></a>
### NanSetIsolateData(v8::Isolate *, T*)

Replaces `v8::Isolate::SetData`.

<a name="api_nan_encode"></a>
### NanEncode(const void*, size_t[, enum Nan::Encoding])

Replaces `node::Encode`.

<a name="api_nan_decode_bytes"></a>
### NanDecodeBytes(v8::Handle&lt;v8::Value&gt;[, enum Nan::Encoding])

Replaces `node::DecodeBytes`.

<a name="api_nan_decode_write"></a>
### NanDecodeWrite(char *, size_t, v8::Handle&lt;v8::Value&gt;[, enum Nan::Encoding])

Replaces `node::DecodeWrite`.

<a name="api_nan_compile_script"></a>
### NanCompileScript(Handle<String> s [, const ScriptOrigin&amp; origin])

Use to create new scripts bound to the current context.

<a name="api_nan_run_script"></a>
### NanRunScript(script)

Use to run both bound and unbound scripts.

<a name="api_nan_callback"></a>
### NanCallback

Because of the difficulties imposed by the changes to `Persistent` handles in V8 in Node 0.11, creating `Persistent` versions of your `Handle<Function>` is annoyingly tricky. `NanCallback` makes it easier by taking your handle, making it persistent until the `NanCallback` is deleted and even providing a handy `Call()` method to fetch and execute the callback `Function`.

```c++
Local<Function> callbackHandle = args[0].As<Function>();
NanCallback *callback = new NanCallback(callbackHandle);
// pass `callback` around and it's safe from GC until you:
delete callback;
```

You can execute the callback like so:

```c++
// no arguments:
callback->Call(0, NULL);

// an error argument:
Handle<Value> argv[] = {
  NanError(NanNew<String>("fail!"))
};
callback->Call(1, argv);

// a success argument:
Handle<Value> argv[] = {
  NanNull(),
  NanNew<String>("w00t!")
};
callback->Call(2, argv);
```

`NanCallback` also has a `Local<Function> GetFunction()` method that you can use
to fetch a local handle to the underlying callback function, as well  as a
`void SetFunction(Handle<Function>)` for setting the callback on the
`NanCallback`. You can check if a `NanCallback` is empty with the `bool IsEmpty()` method. Additionally a generic constructor is available for using
`NanCallback` without performing heap allocations.

<a name="api_nan_async_worker"></a>
### NanAsyncWorker

`NanAsyncWorker` is an abstract class that you can subclass to have much of the annoying async queuing and handling taken care of for you. It can even store arbitrary V8 objects for you and have them persist while the async work is in progress.

See a rough outline of the implementation:

```c++
class NanAsyncWorker {
public:
  NanAsyncWorker (NanCallback *callback);

  // Clean up persistent handles and delete the *callback
  virtual ~NanAsyncWorker ();

  // Check the `ErrorMessage()` and call HandleOKCallback()
  // or HandleErrorCallback depending on whether it has been set or not
  virtual void WorkComplete ();

  // You must implement this to do some async work. If there is an
  // error then use `SetErrorMessage()` to set an error message and the callback will
  // be passed that string in an Error object
  virtual void Execute ();

  // Save a V8 object in a Persistent handle to protect it from GC
  void SaveToPersistent(const char *key, Local<Object> &obj);

  // Fetch a stored V8 object (don't call from within `Execute()`)
  Local<Object> GetFromPersistent(const char *key);

  // Get the error message (or NULL)
  const char *ErrorMessage();

  // Set an error message
  void SetErrorMessage(const char *msg);

protected:
  // Default implementation calls the callback function with no arguments.
  // Override this to return meaningful data
  virtual void HandleOKCallback ();

  // Default implementation calls the callback function with an Error object
  // wrapping the `errmsg` string
  virtual void HandleErrorCallback ();
};
```

<a name="api_nan_async_queue_worker"></a>
### NanAsyncQueueWorker(NanAsyncWorker *)

`NanAsyncQueueWorker` will run a `NanAsyncWorker` asynchronously via libuv. Both the *execute* and *after_work* steps are taken care of for you&mdash;most of the logic for this is embedded in `NanAsyncWorker`.

<a name="api"></a>
### Tests

To run the NAN tests do:

``` sh
npm install
npm run-script rebuild-tests
npm test
```

Or just:

``` sh
npm install
make test
```

## Porting

Follows is a guide to porting your C++ node to NAN.

<table><tbody>

<tr><th>Aspect</th><th>Without NAN</th><th>With NAN</th></tr>

<tr><td>Function declaration</td><td><pre>class MyClass : public ObjectWrap {
    ....
    static Handle<Value> ToString(const Arguments& args);
    ....
}</pre></td></th><td><pre>class MyClass : public ObjectWrap {
    ....
    static NAN_METHOD(ToString);
    ....
}</pre></td></tr>

<tr><td>Function definition</td><td><pre>Handle<Value>
MyClass::ToString(const Arguments& args)
{
	HandleScope scope;
	....
}</pre></td></th><td><pre>
NAN_METHOD(MyClass::ToString)
{
  NanScope();
  ....
}</pre></td></tr>

<tr><td>Return a value</td><td><pre>return scope.Close(result);</pre></td></th><td><pre>NanReturnValue(result);</pre></td></tr>

<tr><td>Return undefined</td><td><pre>return Undefined();</pre></td></th><td><pre>NanReturnUndefined();</pre></td></tr>

<tr><td>Throw an error</td><td><pre>return ThrowException(
  Exception::Error(
    String::New("Error text...")));</pre></td></th><td><pre>return NanThrowError("Error text...");</pre></td></tr>

<tr><td>New v8 string literal</td><td><pre>String::NewSymbol("my text")</pre></td></th><td><pre>NanNew("my text")</pre></td></tr>

</tbody></table>

## Contributors

<a name="governance"></a>

## Governance & Contributing

NAN is governed by the [io.js](https://iojs.org/) Addon API Working Group

### Addon API Working Group (WG)

The NAN project is jointly governed by a Working Group which is responsible for high-level guidance of the project.

Members of the WG are also known as Collaborators, there is no distinction between the two, unlike other io.js projects.

The WG has final authority over this project including:

* Technical direction
* Project governance and process (including this policy)
* Contribution policy
* GitHub repository hosting
* Maintaining the list of additional Collaborators

For the current list of WG members, see the project [README.md](./README.md#collaborators).

Individuals making significant and valuable contributions are made members of the WG and given commit-access to the project. These individuals are identified by the WG and their addition to the WG is discussed via GitHub and requires unanimous consensus amongst those WG members participating in the discussion with a quorum of 50% of WG members required for acceptance of the vote.

_Note:_ If you make a significant contribution and are not considered for commit-access log an issue or contact a WG member directly.

For the current list of WG members / Collaborators, see the project [README.md](./README.md#collaborators).

### Consensus Seeking Process

The WG follows a [Consensus Seeking](http://en.wikipedia.org/wiki/Consensus-seeking_decision-making) decision making model.

Modifications of the contents of the NAN repository are made on a collaborative basis. Anybody with a GitHub account may propose a modification via pull request and it will be considered by the WG. All pull requests must be reviewed and accepted by a WG member with sufficient expertise who is able to take full responsibility for the change. In the case of pull requests proposed by an existing WG member, an additional WG member is required for sign-off. Consensus should be sought if additional WG members participate and there is disagreement around a particular modification.

If a change proposal cannot reach a consensus, a WG member can call for a vote amongst the members of the WG. Simple majority wins.

### Developer's Certificate of Origin 1.0

By making a contribution to this project, I certify that:

* (a) The contribution was created in whole or in part by me and I have the right to submit it under the open source license indicated in the file; or
* (b) The contribution is based upon previous work that, to the best of my knowledge, is covered under an appropriate open source license and I have the right under that license to submit that work with modifications, whether created in whole or in part by me, under the same open source license (unless I am permitted to submit under a different license), as indicated in the file; or
* (c) The contribution was provided directly to me by some other person who certified (a), (b) or (c) and I have not modified it.

<a name="collaborators"></a>
### WG Members / Collaborators

<table><tbody>
<tr><th align="left">Rod Vagg</th><td><a href="https://github.com/rvagg">GitHub/rvagg</a></td><td><a href="http://twitter.com/rvagg">Twitter/@rvagg</a></td></tr>
<tr><th align="left">Benjamin Byholm</th><td><a href="https://github.com/kkoopa/">GitHub/kkoopa</a></td><td>-</td></tr>
<tr><th align="left">Trevor Norris</th><td><a href="https://github.com/trevnorris">GitHub/trevnorris</a></td><td><a href="http://twitter.com/trevnorris">Twitter/@trevnorris</a></td></tr>
<tr><th align="left">Nathan Rajlich</th><td><a href="https://github.com/TooTallNate">GitHub/TooTallNate</a></td><td><a href="http://twitter.com/TooTallNate">Twitter/@TooTallNate</a></td></tr>
<tr><th align="left">Brett Lawson</th><td><a href="https://github.com/brett19">GitHub/brett19</a></td><td><a href="http://twitter.com/brett19x">Twitter/@brett19x</a></td></tr>
<tr><th align="left">Ben Noordhuis</th><td><a href="https://github.com/bnoordhuis">GitHub/bnoordhuis</a></td><td><a href="http://twitter.com/bnoordhuis">Twitter/@bnoordhuis</a></td></tr>
<tr><th align="left">David Siegel</th><td><a href="https://github.com/agnat">GitHub/agnat</a></td><td>-</td></tr>
</tbody></table>

## Licence &amp; copyright

Copyright (c) 2015 NAN WG Members / Collaborators (listed above).

Native Abstractions for Node.js is licensed under an MIT license. All rights not explicitly granted in the MIT license are reserved. See the included LICENSE file for more details.
