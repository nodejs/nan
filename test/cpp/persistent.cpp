/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <cstring>  // memset()

using namespace Nan;  // NOLINT(build/namespaces)

static Persistent<v8::String> persistentTest1;

NAN_METHOD(Save1) {
  persistentTest1.Reset(To<v8::String>(info[0]).ToLocalChecked());
}

NAN_METHOD(Get1) {
  info.GetReturnValue().Set(New(persistentTest1));
}

NAN_METHOD(Dispose1) {
  persistentTest1.Reset();
}

NAN_METHOD(ToPersistentAndBackAgain) {
  Persistent<v8::Object> persistent(To<v8::Object>(info[0]).ToLocalChecked());
  v8::Local<v8::Object> object = New(persistent);
  persistent.Reset();
  memset(&persistent, -1, sizeof(persistent));  // Clobber it good.
  info.GetReturnValue().Set(object);
}

NAN_METHOD(PersistentToPersistent) {
  Persistent<v8::String> persistent(To<v8::String>(info[0]).ToLocalChecked());
  persistentTest1.Reset(persistent);
  persistent.Reset();
  info.GetReturnValue().Set(New(persistentTest1));
  persistentTest1.Reset();
}

NAN_METHOD(CopyablePersistent) {
  CopyablePersistentTraits<v8::String>::CopyablePersistent p;
  p = persistentTest1;
  info.GetReturnValue().Set(New(p));
}

template<typename T> Global<T> passer(v8::Local<T> handle) {
  return Global<T>(handle).Pass();
}

NAN_METHOD(PassGlobal) {
  info.GetReturnValue().Set(passer(New(42)));
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("save1").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(Save1)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("get1").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(Get1)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("dispose1").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(Dispose1)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("toPersistentAndBackAgain").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ToPersistentAndBackAgain))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("persistentToPersistent").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(PersistentToPersistent))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("copyablePersistent").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CopyablePersistent))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("passGlobal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(PassGlobal))
        .ToLocalChecked()
  );
}

NODE_MODULE(persistent, Init)
