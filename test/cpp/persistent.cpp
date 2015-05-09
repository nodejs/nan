/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <cstring>  // memset()

static NanPersistent<v8::String> persistentTest1;

NAN_METHOD(Save1) {
  persistentTest1.Reset(info[0].As<v8::String>());
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Get1) {
  info.GetReturnValue().Set(NanNew(persistentTest1));
}

NAN_METHOD(Dispose1) {
  persistentTest1.Reset();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(ToPersistentAndBackAgain) {
  NanPersistent<v8::Object> persistent(info[0].As<v8::Object>());
  v8::Local<v8::Object> object = NanNew(persistent);
  persistent.Reset();
  memset(&persistent, -1, sizeof(persistent));  // Clobber it good.
  info.GetReturnValue().Set(object);
}

NAN_METHOD(PersistentToPersistent) {
  NanPersistent<v8::String> persistent(info[0].As<v8::String>());
  persistentTest1.Reset(persistent);
  persistent.Reset();
  persistentTest1.Reset();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(CopyablePersistent) {
  NanCopyablePersistentTraits<v8::String>::CopyablePersistent p;
  p = persistentTest1;
  info.GetReturnValue().Set(NanNew(p));
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanNew<v8::String>("save1")
    , NanNew<v8::FunctionTemplate>(Save1)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("get1")
    , NanNew<v8::FunctionTemplate>(Get1)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("dispose1")
    , NanNew<v8::FunctionTemplate>(Dispose1)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("toPersistentAndBackAgain")
    , NanNew<v8::FunctionTemplate>(ToPersistentAndBackAgain)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("persistentToPersistent")
    , NanNew<v8::FunctionTemplate>(PersistentToPersistent)->GetFunction()
  );
  target->Set(
      NanNew<v8::String>("copyablePersistent")
    , NanNew<v8::FunctionTemplate>(CopyablePersistent)->GetFunction()
  );
}

NODE_MODULE(persistent, Init)
