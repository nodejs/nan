/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include "multifile2.h"  // NOLINT(build/include)

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(ReturnString) {
  v8::Local<v8::String> s = New(*Utf8String(info[0])).ToLocalChecked();
  info.GetReturnValue().Set(s);
}
