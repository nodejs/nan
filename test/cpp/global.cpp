/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

template<typename T> Global<T> passer(v8::Local<T> handle) {
  fprintf(stderr, "passer\n");
  return Global<T>(handle).Pass();
}

NAN_METHOD(PassGlobal) {
  fprintf(stderr, "passing global\n");
  info.GetReturnValue().Set(passer(New(42)));
}

NAN_METHOD(EmptyGlobal) {
  Global<v8::String> g(New("value").ToLocalChecked());
  bool b1 = !g.IsEmpty();
  g.Empty();  // this will leak, never do it
  info.GetReturnValue().Set(b1 && g.IsEmpty());
}

// should only work for non-const Global (ref)
// without C++11, types cannot differ (without an explicit cast)
// should move
NAN_METHOD(AssignInitGlobalFromTemporary) {
  fprintf(stderr, "AssignInitGlobalFromTemporary\n");
  fprintf(stderr, "should move\n");
  Global<v8::String> g =
      Global<v8::String>(New("value").ToLocalChecked());
  info.GetReturnValue().Set(!g.IsEmpty());
}

// should only work for non-const Global (ref) of same type 
// without C++11, types cannot differ (without an explicit cast)
// shoudl move
NAN_METHOD(AssignInitGlobalFromLVal) {
  fprintf(stderr, "AssignInitGlobalFromLVal\n");
  fprintf(stderr, "should move\n");
  Global<v8::String> g1(New("value").ToLocalChecked());
  Global<v8::String> g2 = g1;
  info.GetReturnValue().Set(g1.IsEmpty() && !g2.IsEmpty());
}

// should conly work for non-const Global (ref)
// should work for any convertible Global type
// should move
NAN_METHOD(AssignGlobalFromTemporary) {
  fprintf(stderr, "AssignGlobalFromTemporary\n");
  fprintf(stderr, "should move\n");
  Global<v8::Value> g;
  bool b1 = g.IsEmpty();
  g = Global<v8::String>(New("value").ToLocalChecked());
  info.GetReturnValue().Set(b1 && !g.IsEmpty());
}

// should conly work for non-const Global (ref)
// should work for any convertible Global type
// should move
NAN_METHOD(AssignGlobalFromLVal) {
  fprintf(stderr, "AssignGlobalFromLVal\n");
  fprintf(stderr, "should move\n");
  Global<v8::String> g1(New("value").ToLocalChecked());
  bool b1 = !g1.IsEmpty();
  Global<v8::Value> g2;
  bool b2 = g2.IsEmpty();
  g2 = g1;
  info.GetReturnValue().Set(b1 && b2 && g1.IsEmpty() && !g2.IsEmpty());
}

// should work for any convertible type
// should move
NAN_METHOD(ConstructGlobalFromNonConstGlobalTemporary) {
  fprintf(stderr, "ConstructGlobalFromNonConstGlobalTemporary\n");
  fprintf(stderr, "should move\n");
  Global<v8::Value> g(Global<v8::String>(New("value").ToLocalChecked()));
  info.GetReturnValue().Set(!g.IsEmpty());
}

// should work for any convertible type
// should move
NAN_METHOD(ConstructGlobalFromNonConstGlobalLVal) {
  fprintf(stderr, "ConstructGlobalFromNonConstGlobalLVal\n");
  fprintf(stderr, "should move\n");
  Global<v8::String> g1(New("value").ToLocalChecked());
  Global<v8::Value> g2(g1);
  info.GetReturnValue().Set(g1.IsEmpty() && !g2.IsEmpty());
}

template<typename T>
const Global<T> constReturner(v8::Local<T> handle) {
  return Global<T>(handle);
}

// should work for any convertible type
// should copy
NAN_METHOD(ConstructGlobalFromConstGlobalTemporary) {
  fprintf(stderr, "ConstructGlobalFromConstGlobalTemporary\n");
  fprintf(stderr, "should copy\n");
  Global<v8::Value> g(constReturner(New("value").ToLocalChecked()));
  info.GetReturnValue().Set(!g.IsEmpty());
}

// should work for any convertible type
// should copy
NAN_METHOD(ConstructGlobalFromConstGlobalLVal) {
  fprintf(stderr, "ConstructGlobalFromConstGlobalLVal\n");
  fprintf(stderr, "should copy\n");
  const Global<v8::String> g1(New("value").ToLocalChecked());
  Global<v8::Value> g2(g1);
  info.GetReturnValue().Set(!g1.IsEmpty() && !g2.IsEmpty());
}

//should work for any convertible type
//should move
NAN_METHOD(ConstructGlobalFromNonConstPersistentBaseTemporary) {
  fprintf(stderr, "ConstructGlobalFromNonConstPersistentBaseTemporary\n");
  fprintf(stderr, "should move\n");
  Global<v8::Value> g(Persistent<v8::String>(New("value").ToLocalChecked()));
  info.GetReturnValue().Set(!g.IsEmpty());
}

//should work for any convertible type
//should copy
NAN_METHOD(ConstructGlobalFromNonConstPersistentBaseLVal) {
  fprintf(stderr, "ConstructGlobalFromNonConstPersistentBaseLVal\n");
  fprintf(stderr, "should copy\n");
  Persistent<v8::String> p(New("value").ToLocalChecked());
  Global<v8::Value> g(p);
  info.GetReturnValue().Set(!p.IsEmpty() && !g.IsEmpty());
  p.Reset();
}

template<typename T>
const typename CopyablePersistentTraits<T>::CopyablePersistent
    constPersistentReturner(v8::Local<T> handle) {
  return typename CopyablePersistentTraits<T>::CopyablePersistent(handle);
}

//should work for any convertible type
//should copy
NAN_METHOD(ConstructGlobalFromConstPersistentBaseTemporary) {
  fprintf(stderr, "ConstructGlobalFromConstPersistentBaseTemporary\n");
  fprintf(stderr, "should copy\n");
  Global<v8::Value> g(constPersistentReturner(New("value").ToLocalChecked()));
  info.GetReturnValue().Set(!g.IsEmpty());
}

//should work for any convertible type
//should copy
NAN_METHOD(ConstructGlobalFromConstPersistentBaseLVal) {
  fprintf(stderr, "ConstructGlobalFromConstPersistentBaseLVal\n");
  fprintf(stderr, "should copy\n");
  const Persistent<v8::String> p(New("value").ToLocalChecked());
  Global<v8::Value> g(p);
  info.GetReturnValue().Set(!p.IsEmpty() && !g.IsEmpty());
  const_cast<Persistent<v8::String> &>(p).Reset();
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("passGlobal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(PassGlobal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("emptyGlobal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(EmptyGlobal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("assignInitGlobalFromTemporary").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(AssignInitGlobalFromTemporary))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("assignInitGlobalFromLVal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(AssignInitGlobalFromLVal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("assignGlobalFromTemporary").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(AssignGlobalFromTemporary))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("assignGlobalFromLVal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(AssignGlobalFromLVal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("constructGlobalFromNonConstGlobalTemporary").ToLocalChecked()
    ,
GetFunction(New<v8::FunctionTemplate>(ConstructGlobalFromNonConstGlobalTemporary))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("constructGlobalFromNonConstGlobalLVal").ToLocalChecked()
    ,
GetFunction(New<v8::FunctionTemplate>(ConstructGlobalFromNonConstGlobalLVal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("constructGlobalFromConstGlobalTemporary").ToLocalChecked()
    ,
GetFunction(New<v8::FunctionTemplate>(ConstructGlobalFromConstGlobalTemporary))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("constructGlobalFromConstGlobalLVal").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ConstructGlobalFromConstGlobalLVal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("constructGlobalFromNonConstPersistentBaseTemporary").ToLocalChecked()
    ,
GetFunction(New<v8::FunctionTemplate>(ConstructGlobalFromNonConstPersistentBaseTemporary))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("constructGlobalFromNonConstPersistentBaseLVal").ToLocalChecked()
    ,
GetFunction(New<v8::FunctionTemplate>(ConstructGlobalFromNonConstPersistentBaseLVal))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("constructGlobalFromConstPersistentBaseTemporary").ToLocalChecked()
    ,
GetFunction(New<v8::FunctionTemplate>(ConstructGlobalFromConstPersistentBaseTemporary))
        .ToLocalChecked()
  );
  Set(target
    , New<v8::String>("constructGlobalFromConstPersistentBaseLVal").ToLocalChecked()
    ,
GetFunction(New<v8::FunctionTemplate>(ConstructGlobalFromConstPersistentBaseLVal))
        .ToLocalChecked()
  );
}

NODE_MODULE(global, Init)
