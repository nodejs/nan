/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

void Init (v8::Handle<v8::Object> target) {
  NanSet(target
    , NanNew<v8::String>("key").ToLocalChecked()
    , NanNew<v8::String>("a property").ToLocalChecked()
  );
}

NODE_MODULE(symbols, Init)
