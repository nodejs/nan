/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>
#include "./sync.h"
#include "./async.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;

// Expose synchronous and asynchronous access to our
// Estimate() function
void InitAll(Handle<Object> exports) {
  exports->Set(NanNew<String>("calculateSync"),
    NanNew<FunctionTemplate>(CalculateSync)->GetFunction());

  exports->Set(NanNew<String>("calculateAsync"),
    NanNew<FunctionTemplate>(CalculateAsync)->GetFunction());
}

NODE_MODULE(addon, InitAll)
