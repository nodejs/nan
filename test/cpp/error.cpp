/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

#define X(NAME)                                                                \
  NAN_METHOD(Throw ## NAME ## 1) {                                             \
    return Nan::Throw ## NAME("errmsg");                                       \
  }                                                                            \
                                                                               \
  NAN_METHOD(Throw ## NAME ## 2) {                                             \
    return Nan::Throw ## NAME(Nan::New("errmsg").ToLocalChecked());            \
  }                                                                            \
                                                                               \
  NAN_METHOD(Throw ## NAME ## 3) {                                             \
    return Nan::ThrowError(NAME("errmsg"));                                    \
  }                                                                            \
                                                                               \
  NAN_METHOD(Throw ## NAME ## 4) {                                             \
    return Nan::ThrowError(NAME(Nan::New("errmsg").ToLocalChecked()));         \
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
  Nan::Set(                                                                    \
      target                                                                   \
    , Nan::New(#NAME).ToLocalChecked()                                         \
    , Nan::GetFunction(New<v8::FunctionTemplate>(NAME)).ToLocalChecked());


NAN_MODULE_INIT(Init) {
  EXPORT_ERROR_FUNCTIONS(Error)
  EXPORT_ERROR_FUNCTIONS(RangeError)
  EXPORT_ERROR_FUNCTIONS(ReferenceError)
  EXPORT_ERROR_FUNCTIONS(SyntaxError)
  EXPORT_ERROR_FUNCTIONS(TypeError)
}

#undef EXPORT_ERROR_FUNCTIONS
#undef X

NODE_MODULE(error, Init)
