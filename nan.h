/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2013 Rod Vagg
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 *
 * Version 0.0.1
 * See https://github.com/rvagg/nan for the latest update to this file
 **********************************************************************************/

#ifndef NAN_H
#define NAN_H

#include <node.h>
#include <node_buffer.h>

// some generic helpers

#define NanSymbol(value) v8::String::NewSymbol(value)

static inline char* NanFromV8String(v8::Local<v8::Value> from) {
  size_t sz_;
  char* to;
  v8::Local<v8::String> toStr = from->ToString();
  sz_ = toStr->Utf8Length();
  to = new char[sz_ + 1];
  toStr->WriteUtf8(to, -1, NULL, v8::String::NO_OPTIONS);
  return to;
}

static inline bool NanBooleanOptionValue(
      v8::Local<v8::Object> optionsObj
    , v8::Handle<v8::String> opt, bool def) {

  if (def) {
    return optionsObj.IsEmpty()
      || !optionsObj->Has(opt)
      || optionsObj->Get(opt)->BooleanValue();
  } else {
    return !optionsObj.IsEmpty()
      && optionsObj->Has(opt)
      && optionsObj->Get(opt)->BooleanValue();
  }
}

static inline bool NanBooleanOptionValue(
      v8::Local<v8::Object> optionsObj
    , v8::Handle<v8::String> opt) {
  return NanBooleanOptionValue(optionsObj, opt, false);
}

static inline uint32_t NanUInt32OptionValue(
      v8::Local<v8::Object> optionsObj
    , v8::Handle<v8::String> opt
    , uint32_t def) {

  return !optionsObj.IsEmpty()
    && optionsObj->Has(opt)
    && optionsObj->Get(opt)->IsUint32()
      ? optionsObj->Get(opt)->Uint32Value()
      : def;
}

#if (NODE_MODULE_VERSION > 0x000B)
// Node 0.11+ (0.11.3 and below won't compile with these)

static v8::Isolate* nan_isolate = v8::Isolate::GetCurrent();

# define NAN_METHOD(name)                                                      \
    void name(const v8::FunctionCallbackInfo<v8::Value>& args)

# define NanScope() v8::HandleScope scope(nan_isolate)
# define NanReturnValue(value) return args.GetReturnValue().Set(value);
# define NanReturnUndefined() return;
# define NanAssignPersistent(type, handle, obj) handle.Reset(nan_isolate, obj);
# define NanObjectWrapHandle(obj) obj->handle()

# define THROW_ERROR(fun, errmsg)                                              \
    do {                                                                       \
      NanScope();                                                              \
      v8::ThrowException(fun(v8::String::New(errmsg)));                        \
    } while (0);

  inline static void NanThrowError(const char* errmsg) {
    THROW_ERROR(v8::Exception::Error, errmsg);
  }

  inline static void NanThrowTypeError(const char* errmsg) {
    THROW_ERROR(v8::Exception::TypeError, errmsg);
  }

  inline static void NanThrowRangeError(const char* errmsg) {
    THROW_ERROR(v8::Exception::RangeError, errmsg);
  }

  static inline void NanDispose(v8::Persistent<v8::Object> &handle) {
    handle.Dispose(nan_isolate);
  }

  static inline v8::Local<v8::Object> NanNewBufferHandle (
     char *data, uint32_t size) {
    return node::Buffer::New(data, size);
  }

  template <class TypeName>
  inline v8::Local<TypeName> NanPersistentToLocal(
     const v8::Persistent<TypeName>& persistent) {
    if (persistent.IsWeak()) {
     return v8::Local<TypeName>::New(nan_isolate, persistent);
    } else {
     return *reinterpret_cast<v8::Local<TypeName>*>(
         const_cast<v8::Persistent<TypeName>*>(&persistent));
    }
  }

#else
// Node 0.8 and 0.10

# define NAN_METHOD(name)                                                      \
    v8::Handle<v8::Value> name(const v8::Arguments& args)

# define NanScope() v8::HandleScope scope
# define NanReturnValue(value) return scope.Close(value);
# define NanReturnUndefined() return v8::Undefined();
# define NanAssignPersistent(type, handle, obj)                                \
    handle = v8::Persistent<type>::New(obj);
# define NanObjectWrapHandle(obj) obj->handle_

# define THROW_ERROR(fun, errmsg)                                              \
    do {                                                                       \
      NanScope();                                                              \
      return v8::ThrowException(fun(v8::String::New(errmsg)));                 \
    } while (0);

  inline static v8::Handle<v8::Value> NanThrowError(const char* errmsg) {
    THROW_ERROR(v8::Exception::Error, errmsg);
  }

  inline static v8::Handle<v8::Value> NanThrowTypeError(const char* errmsg) {
    THROW_ERROR(v8::Exception::TypeError, errmsg);
  }

  inline static v8::Handle<v8::Value> NanThrowRangeError(const char* errmsg) {
    THROW_ERROR(v8::Exception::RangeError, errmsg);
  }

  static inline void NanDispose(v8::Persistent<v8::Object> &handle) {
    handle.Dispose();
  }

  static inline v8::Local<v8::Object> NanNewBufferHandle (
     char *data, uint32_t size) {
    return v8::Local<v8::Object>::New(node::Buffer::New(data, size)->handle_);
  }

  template <class TypeName>
  inline v8::Local<TypeName> NanPersistentToLocal(
     const v8::Persistent<TypeName>& persistent) {
    if (persistent.IsWeak()) {
     return v8::Local<TypeName>::New(persistent);
    } else {
     return *reinterpret_cast<v8::Local<TypeName>*>(
         const_cast<v8::Persistent<TypeName>*>(&persistent));
    }
  }

#endif // node version

class NanCallback {
 public:
  NanCallback(const v8::Local<v8::Function> &fn) {
   NanScope();
   v8::Local<v8::Object> obj = v8::Object::New();
   obj->Set(NanSymbol("callback"), fn);
   NanAssignPersistent(v8::Object, handle, obj);
  }

  ~NanCallback() {
   if (handle.IsEmpty()) return;
   handle.Dispose();
  }

  void Run(int argc, v8::Local<v8::Value> argv[]) {
   NanScope();
   v8::Local<v8::Function> callback = NanPersistentToLocal(handle)->
       Get(NanSymbol("callback")).As<v8::Function>();
   v8::TryCatch try_catch;
   callback->Call(v8::Context::GetCurrent()->Global(), argc, argv);
   if (try_catch.HasCaught()) {
     node::FatalException(try_catch);
   }
  }

 private:
  v8::Persistent<v8::Object> handle;
};

/* abstract */ class NanAsyncWorker {
public:
  NanAsyncWorker (NanCallback *callback) : callback(callback) {
    request.data = this;
    errmsg = NULL;
  }

  virtual ~NanAsyncWorker () {
    if (!persistentHandle.IsEmpty())
      NanDispose(persistentHandle);
    if (callback)
      delete callback;
  }

  virtual void WorkComplete () {
    NanScope();

    if (errmsg == NULL)
      HandleOKCallback();
    else
      HandleErrorCallback();
    delete callback;
    callback = NULL;
  }

  virtual void Execute () =0;

  uv_work_t request;

protected:
  v8::Persistent<v8::Object> persistentHandle;
  NanCallback *callback;
  const char *errmsg;

  void SavePersistent(const char *key, v8::Local<v8::Object> &obj) {
    v8::Local<v8::Object> handle = NanPersistentToLocal(persistentHandle);
    handle->Set(NanSymbol(key), obj);
  }

  v8::Local<v8::Object> GetFromPersistent(const char *key) {
    v8::Local<v8::Object> handle = NanPersistentToLocal(persistentHandle);
    return handle->Get(NanSymbol(key)).As<v8::Object>();
  }

  virtual void HandleOKCallback () {
    NanScope();

    callback->Run(0, NULL);
  };

  virtual void HandleErrorCallback () {
    NanScope();

    v8::Local<v8::Value> argv[] = {
        v8::Exception::Error(v8::String::New(errmsg))
    };
    callback->Run(1, argv);
  }
};

inline void NanAsyncExecute (uv_work_t* req) {
  NanAsyncWorker *worker = static_cast<NanAsyncWorker*>(req->data);
  worker->Execute();
}

inline void NanAsyncExecuteComplete (uv_work_t* req) {
  NanAsyncWorker* worker = static_cast<NanAsyncWorker*>(req->data);
  worker->WorkComplete();
  delete worker;
}

inline void NanAsyncQueueWorker (NanAsyncWorker* worker) {
  uv_queue_work(
      uv_default_loop()
    , &worker->request
    , NanAsyncExecute
    , (uv_after_work_cb)NanAsyncExecuteComplete
  );
}

#endif
