/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

#define _(e) NAN_TEST_EXPRESSION(e)

// Based on test-thread.c from libuv.

class TlsTest : public AsyncWorker {
 public:
  explicit TlsTest(Tap *t) : AsyncWorker(NULL), t(t), i(0) {
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
    HandleScope scope;
    for (unsigned j = 0; j < i; ++j)
      t->ok(res[j].ok, res[j].msg);
    nauv_key_delete(&tls_key);
    t->ok(_(NULL == ErrorMessage()));
    t->end();
    delete t;
  }

 private:
  nauv_key_t tls_key;

  Tap *t;
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
  Tap *t = new Tap(info[0]);
  AsyncQueueWorker(new TlsTest(t));
  info.GetReturnValue().SetUndefined();
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("thread_local_storage").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(thread_local_storage))
        .ToLocalChecked()
  );
}

NODE_MODULE(threadlocal, Init)
