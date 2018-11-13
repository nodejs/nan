/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(ReturnUndefined) {
  info.GetReturnValue().SetUndefined();
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("r").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(ReturnUndefined)).ToLocalChecked()
  );
}

NODE_MODULE(returnundefined, Init)

