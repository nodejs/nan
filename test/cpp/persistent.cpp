/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <cstring>  // memset()

using namespace Nan;  // NOLINT(build/namespaces)

static Persistent<v8::String> persistentTest1;

NAN_METHOD(Save1) {
  persistentTest1.Reset(info[0].As<v8::String>());
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(Get1) {
  info.GetReturnValue().Set(New(persistentTest1));
}

NAN_METHOD(Dispose1) {
  persistentTest1.Reset();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(ToPersistentAndBackAgain) {
  Persistent<v8::Object> persistent(info[0].As<v8::Object>());
  v8::Local<v8::Object> object = New(persistent);
  persistent.Reset();
  memset(&persistent, -1, sizeof(persistent));  // Clobber it good.
  info.GetReturnValue().Set(object);
}

NAN_METHOD(PersistentToPersistent) {
  Persistent<v8::String> persistent(info[0].As<v8::String>());
  persistentTest1.Reset(persistent);
  persistent.Reset();
  persistentTest1.Reset();
  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(CopyablePersistent) {
  CopyablePersistentTraits<v8::String>::CopyablePersistent p;
  p = persistentTest1;
  info.GetReturnValue().Set(New(p));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("save1").ToLocalChecked()
    , New<v8::FunctionTemplate>(Save1)->GetFunction()
  );
  Set(target
    , New<v8::String>("get1").ToLocalChecked()
    , New<v8::FunctionTemplate>(Get1)->GetFunction()
  );
  Set(target
    , New<v8::String>("dispose1").ToLocalChecked()
    , New<v8::FunctionTemplate>(Dispose1)->GetFunction()
  );
  Set(target
    , New<v8::String>("toPersistentAndBackAgain").ToLocalChecked()
    , New<v8::FunctionTemplate>(ToPersistentAndBackAgain)->GetFunction()
  );
  Set(target
    , New<v8::String>("persistentToPersistent").ToLocalChecked()
    , New<v8::FunctionTemplate>(PersistentToPersistent)->GetFunction()
  );
  Set(target
    , New<v8::String>("copyablePersistent").ToLocalChecked()
    , New<v8::FunctionTemplate>(CopyablePersistent)->GetFunction()
  );
}

NODE_MODULE(persistent, Init)
