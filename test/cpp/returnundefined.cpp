/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <node.h>
#include <nan.h>

NAN_METHOD(ReturnUndefined) {
  NanScope();
  NanReturnUndefined();
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , NanNew<v8::FunctionTemplate>(ReturnUndefined)->GetFunction()
  );
}

NODE_MODULE(returnundefined, Init)

