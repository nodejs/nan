/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("key").ToLocalChecked()
    , New<v8::String>("a property").ToLocalChecked()
  );
}

NODE_MODULE(symbols, Init)
