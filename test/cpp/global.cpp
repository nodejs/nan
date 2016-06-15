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
  return Global<T>(handle).Pass();
}

NAN_METHOD(PassGlobal) {
  info.GetReturnValue().Set(passer(New(42)));
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

NODE_MODULE(global, Init)
