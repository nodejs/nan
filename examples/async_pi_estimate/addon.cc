/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <node.h>
#include <nan.h>
#include "./sync.h"
#include "./async.h"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;

// Expose synchronous and asynchronous access to our
// Estimate() function
void InitAll(Handle<Object> exports) {
  exports->Set(NanSymbol("calculateSync"),
    NanNew<FunctionTemplate>(CalculateSync)->GetFunction());

  exports->Set(NanSymbol("calculateAsync"),
    NanNew<FunctionTemplate>(CalculateAsync)->GetFunction());
}

NODE_MODULE(addon, InitAll)
