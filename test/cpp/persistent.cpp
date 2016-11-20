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

template<typename T> Global<T> passer(v8::Local<T> handle) {
  return Global<T>(handle).Pass();
}

NAN_METHOD(PassGlobal) {
  info.GetReturnValue().Set(passer(New(42)));
}

NAN_METHOD(EmptyPersistent) {
  persistentTest2.Reset(New("value").ToLocalChecked());
  bool b1 = !persistentTest2.IsEmpty();
  persistentTest2.Empty();  // this will leak, never do it
  info.GetReturnValue().Set(b1 && persistentTest2.IsEmpty());
}

NAN_METHOD(EmptyGlobal) {
  Global<v8::String> g(New("value").ToLocalChecked());
  bool b1 = !g.IsEmpty();
  g.Empty();  // this will leak, never do it
  info.GetReturnValue().Set(b1 && g.IsEmpty());
}

NAN_METHOD(MoveConstructGlobal) {
  Global<v8::String> g(Global<v8::String>(New("value").ToLocalChecked()));
  info.GetReturnValue().Set(!g.IsEmpty());
}

NAN_METHOD(CopyConstructGlobal) {
  Persistent<v8::String> p(New("value").ToLocalChecked());
  bool b1 = !p.IsEmpty();
  Global<v8::String> g(p);
  bool b2 = !p.IsEmpty();
  p.Reset();
  info.GetReturnValue().Set(b1 && b2 && !g.IsEmpty());
}

NAN_METHOD(MoveAssignGlobal) {
  Global<v8::String> g = passer(New("value").ToLocalChecked());
  info.GetReturnValue().Set(!g.IsEmpty());
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
  Set(target
    , New<v8::String>("emptyPersistent").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(EmptyPersistent))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("emptyGlobal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(EmptyGlobal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("moveConstructGlobal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(MoveConstructGlobal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("copyConstructGlobal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(CopyConstructGlobal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("moveAssignGlobal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(MoveAssignGlobal))
        .ToLocalChecked()
  );
}

NODE_MODULE(persistent, Init)
