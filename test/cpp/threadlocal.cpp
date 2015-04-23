/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

#define _(e) NAN_TEST_EXPRESSION(e)

// Based on test-thread.c from libuv.

class TlsTest : public NanAsyncWorker {
 public:
  explicit TlsTest(NanTap *t) : NanAsyncWorker(NULL), t(t), i(0) {
    NanScope();
    t->plan(7);
    t->ok(_(0 == nauv_key_create(&tls_key)));
    t->ok(_(NULL == nauv_key_get(&tls_key)));
    nauv_key_set(&tls_key, this);
    t->ok(_(this == nauv_key_get(&tls_key)));
  }
  void Execute() {
    ok(_(NULL == nauv_key_get(&tls_key)));
    nauv_key_set(&tls_key, &i);
    ok(_(&i == nauv_key_get(&tls_key)));
    nauv_key_set(&tls_key, NULL);
    ok(_(NULL == nauv_key_get(&tls_key)));
  }
  void WorkComplete() {
    NanScope();
    for (unsigned j = 0; j < i; ++j)
      t->ok(res[j].ok, res[j].msg);
    nauv_key_delete(&tls_key);
    t->ok(_(NULL == ErrorMessage()));
    delete t;
  }

 private:
  nauv_key_t tls_key;

  NanTap *t;
  struct { bool ok; const char* msg; } res[3];
  unsigned i;
  void ok(bool isOk, const char *msg) {
    assert(i < sizeof(res)/sizeof(res[0]));
    res[i].ok = isOk;
    res[i].msg = msg;
    ++i;
  }
};

NAN_METHOD(thread_local_storage) {
  NanScope();
  NanTap *t = new NanTap(args[0]);
  NanAsyncQueueWorker(new TlsTest(t));
  return_NanUndefined();
}

void Init(v8::Handle<v8::Object> exports) {
  NanSet(exports
    , NanNew<v8::String>("thread_local_storage").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(thread_local_storage)->GetFunction()
  );
}

NODE_MODULE(threadlocal, Init)
