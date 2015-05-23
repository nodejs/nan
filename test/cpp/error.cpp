/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

#define X(NAME)                                                                \
  NAN_METHOD(Throw ## NAME ## 1) {                                             \
    return NanThrow ## NAME("errmsg");                                         \
  }                                                                            \
                                                                               \
  NAN_METHOD(Throw ## NAME ## 2) {                                             \
    return NanThrow ## NAME(NanNew("errmsg").ToLocalChecked());                \
  }                                                                            \
                                                                               \
  NAN_METHOD(Throw ## NAME ## 3) {                                             \
    return NanThrowError(Nan ## NAME("errmsg"));                               \
  }                                                                            \
                                                                               \
  NAN_METHOD(Throw ## NAME ## 4) {                                             \
    return NanThrowError(Nan ## NAME(NanNew("errmsg").ToLocalChecked()));      \
  }

X(Error)
X(RangeError)
X(ReferenceError)
X(SyntaxError)
X(TypeError)

#undef X

#define EXPORT_ERROR_FUNCTIONS(NAME)                                           \
  X(Throw ## NAME ## 1)                                                        \
  X(Throw ## NAME ## 2)                                                        \
  X(Throw ## NAME ## 3)                                                        \
  X(Throw ## NAME ## 4)

#define X(NAME)                                                                \
  NanSet(                                                                      \
      target                                                                   \
    , NanNew(#NAME).ToLocalChecked()                                           \
    , NanNew<v8::FunctionTemplate>(NAME)->GetFunction());


void Init (v8::Handle<v8::Object> target) {
  EXPORT_ERROR_FUNCTIONS(Error)
  EXPORT_ERROR_FUNCTIONS(RangeError)
  EXPORT_ERROR_FUNCTIONS(ReferenceError)
  EXPORT_ERROR_FUNCTIONS(SyntaxError)
  EXPORT_ERROR_FUNCTIONS(TypeError)
}

#undef EXPORT_ERROR_FUNCTIONS
#undef X

NODE_MODULE(error, Init)
