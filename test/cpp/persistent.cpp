/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>
#include <string.h>  // memset()

static v8::Persistent<v8::String> persistentTest1;

NAN_METHOD(Save1) {
  NanScope();

  NanAssignPersistent(persistentTest1, args[0].As<v8::String>());

  NanReturnUndefined();
}

NAN_METHOD(Get1) {
  NanScope();

  NanReturnValue(NanPersistentToLocal<v8::String>(persistentTest1));
}

NAN_METHOD(Dispose1) {
  NanScope();

  NanDisposePersistent(persistentTest1);

  NanReturnUndefined();
}

NAN_METHOD(ToPersistentAndBackAgain) {
  NanScope();

  v8::Persistent<v8::Object> persistent;
  NanAssignPersistent(persistent, args[0].As<v8::Object>());
  v8::Local<v8::Object> object = NanPersistentToLocal<v8::Object>(persistent);
  NanDisposePersistent(persistent);
  memset(&persistent, -1, sizeof(persistent));  // Clobber it good.

  NanReturnValue(object);
}

/* do not ever do like this, it leaks a persistent handle */
NAN_METHOD(InitPersistentAndLeak) {
  NanScope();
  NanInitPersistent(result, v8::String::New("result"));
  NanReturnValue(NanPersistentToLocal(result));
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("save1")
    , v8::FunctionTemplate::New(Save1)->GetFunction()
  );
  target->Set(
      NanSymbol("get1")
    , v8::FunctionTemplate::New(Get1)->GetFunction()
  );
  target->Set(
      NanSymbol("dispose1")
    , v8::FunctionTemplate::New(Dispose1)->GetFunction()
  );
  target->Set(
      NanSymbol("toPersistentAndBackAgain")
    , v8::FunctionTemplate::New(ToPersistentAndBackAgain)->GetFunction()
  );
  target->Set(
      NanSymbol("initPersistentAndLeak")
    , v8::FunctionTemplate::New(InitPersistentAndLeak)->GetFunction()
  );
}

NODE_MODULE(persistent, Init)
