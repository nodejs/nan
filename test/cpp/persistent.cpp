#include <nan.h>
#include <string.h>  // memset()

static v8::Persistent<v8::String> persistentTest1;

NAN_METHOD(Save1) {
  NanScope();

  NanAssignPersistent(v8::String, persistentTest1, args[0].As<v8::String>());

  NanReturnUndefined();
}

NAN_METHOD(Get1) {
  NanScope();

  NanReturnValue(NanPersistentToLocal<v8::String>(persistentTest1));
}

NAN_METHOD(Dispose1) {
  NanScope();

  NanDispose(persistentTest1);

  NanReturnUndefined();
}

NAN_METHOD(ToPersistentAndBackAgain) {
  NanScope();

  v8::Persistent<v8::Object> persistent;
  NanAssignPersistent(v8::Object, persistent, args[0].As<v8::Object>());
  v8::Local<v8::Object> object = NanPersistentToLocal<v8::Object>(persistent);
  NanDispose(persistent);
  memset(&persistent, -1, sizeof(persistent));  // Clobber it good.

  NanReturnValue(object);
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
}

NODE_MODULE(persistent, Init)
