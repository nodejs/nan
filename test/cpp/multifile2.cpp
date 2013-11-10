#include <node.h>
#include "nan.h"

NAN_METHOD(ReturnString) {
  NanScope();

  NanReturnValue(v8::String::New(NanFromV8String(args[0].As<v8::Object>())));
}
