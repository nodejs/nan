/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include <cstring>  // memset()

using namespace Nan;  // NOLINT(build/namespaces)

static Persistent<v8::String> persistentTest1;
static Persistent<v8::String> persistentTest2;

NAN_METHOD(Save1) {
  persistentTest1.Reset(info[0].As<v8::String>());
}

NAN_METHOD(Get1) {
  info.GetReturnValue().Set(New(persistentTest1));
}

NAN_METHOD(Dispose1) {
  persistentTest1.Reset();
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
  info.GetReturnValue().Set(New(persistentTest1));
  persistentTest1.Reset();
}

NAN_METHOD(CopyablePersistent) {
  CopyablePersistentTraits<v8::String>::CopyablePersistent p;
  p = persistentTest1;
  info.GetReturnValue().Set(New(p));
}

NAN_METHOD(EmptyPersistent) {
  persistentTest2.Reset(New("value").ToLocalChecked());
  bool b1 = !persistentTest2.IsEmpty();
  persistentTest2.Empty();  // this will leak, never do it
  info.GetReturnValue().Set(b1 && persistentTest2.IsEmpty());
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
    , New<v8::String>("emptyPersistent").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(EmptyPersistent))
        .ToLocalChecked()
  );
}

NODE_MODULE(persistent, Init)
