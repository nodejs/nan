/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <node_version.h>

#if NODE_MODULE_VERSION >= 12
# define NAN_ENABLE_FUNCTION_LENGTH
# define LENGTH(L) , L
#else
# define LENGTH(L) 
#endif

# include <nan.h>

using namespace v8;  // NOLINT(build/namespaces)

namespace {


NAN_METHOD(lengthFiveFunction) {
  NanScope();
  return_NanValue( NanNew<Function>( lengthFiveFunction
                                   , Handle<Value>()
                                   LENGTH(5)));
}

NAN_METHOD(lengthSevenFunctionFromTemplate) {
  NanScope();
  return_NanValue( NanNew<FunctionTemplate>( lengthSevenFunctionFromTemplate
                                           , Handle<Value>()
                                           , Handle<Signature>()
                                           LENGTH(7))->GetFunction());
}

void Init(Handle<Object> exports) {
  NAN_EXPORT(exports, lengthFiveFunction);
  NAN_EXPORT(exports, lengthSevenFunctionFromTemplate);
}

}  // end of anonymous namespace

NODE_MODULE(nannew_function_length, Init)

