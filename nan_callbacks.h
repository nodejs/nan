/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_CALLBACKS_H_
#define NAN_CALLBACKS_H_

#if defined(_MSC_VER)
# pragma warning( push )
# pragma warning( disable : 4530 )
# include <map>
# pragma warning( pop )
#else
# include <map>
#endif

template<typename T> class NanFunctionCallbackInfo;
template<typename T> class NanPropertyCallbackInfo;
template<typename T> class NanGlobal;

typedef void(*NanFunctionCallback)(const NanFunctionCallbackInfo<v8::Value>&);
typedef void(*NanGetterCallback)
    (v8::Local<v8::String>, const NanPropertyCallbackInfo<v8::Value>&);
typedef void(*NanSetterCallback)(
    v8::Local<v8::String>,
    v8::Local<v8::Value>,
    const NanPropertyCallbackInfo<void>&);
typedef void(*NanPropertyGetterCallback)(
    v8::Local<v8::String>,
    const NanPropertyCallbackInfo<v8::Value>&);
typedef void(*NanPropertySetterCallback)(
    v8::Local<v8::String>,
    v8::Local<v8::Value>,
    const NanPropertyCallbackInfo<v8::Value>&);
typedef void(*NanPropertyEnumeratorCallback)
    (const NanPropertyCallbackInfo<v8::Array>&);
typedef void(*NanPropertyDeleterCallback)(
    v8::Local<v8::String>,
    const NanPropertyCallbackInfo<v8::Boolean>&);
typedef void(*NanPropertyQueryCallback)(
    v8::Local<v8::String>,
    const NanPropertyCallbackInfo<v8::Integer>&);
typedef void(*NanIndexGetterCallback)(
    uint32_t,
    const NanPropertyCallbackInfo<v8::Value>&);
typedef void(*NanIndexSetterCallback)(
    uint32_t,
    v8::Local<v8::Value>,
    const NanPropertyCallbackInfo<v8::Value>&);
typedef void(*NanIndexEnumeratorCallback)
    (const NanPropertyCallbackInfo<v8::Array>&);
typedef void(*NanIndexDeleterCallback)(
    uint32_t,
    const NanPropertyCallbackInfo<v8::Boolean>&);
typedef void(*NanIndexQueryCallback)(
    uint32_t,
    const NanPropertyCallbackInfo<v8::Integer>&);

namespace Nan { namespace imp {
typedef v8::Handle<v8::AccessorSignature> Sig;

#define X(NAME)                                                                \
struct NAME ## Wrapper {                                                       \
  explicit NAME ## Wrapper(Nan ## NAME ## Callback callback_) :                \
      callback(callback_) {}                                                   \
  Nan ## NAME ## Callback callback;                                            \
};

X(Function)
X(Getter)
X(Setter)
X(PropertyGetter)
X(PropertySetter)
X(PropertyEnumerator)
X(PropertyDeleter)
X(PropertyQuery)
X(IndexGetter)
X(IndexSetter)
X(IndexEnumerator)
X(IndexDeleter)
X(IndexQuery)

#undef X

static const int kDataIndex =                    0;

static const int kFunctionIndex =                1;
static const int kFunctionFieldCount =           2;

static const int kGetterIndex =                  1;
static const int kSetterIndex =                  2;
static const int kAccessorFieldCount =           3;

static const int kPropertyGetterIndex =          1;
static const int kPropertySetterIndex =          2;
static const int kPropertyEnumeratorIndex =      3;
static const int kPropertyDeleterIndex =         4;
static const int kPropertyQueryIndex =           5;
static const int kPropertyFieldCount =           6;

static const int kIndexPropertyGetterIndex =     1;
static const int kIndexPropertySetterIndex =     2;
static const int kIndexPropertyEnumeratorIndex = 3;
static const int kIndexPropertyDeleterIndex =    4;
static const int kIndexPropertyQueryIndex =      5;
static const int kIndexPropertyFieldCount =      6;

template<typename T, typename P>
P *GetWrapper(T needle) {
  static std::map<T, P*> haystack;
  typename std::map<T, P*>::iterator it =
      haystack.find(needle);

  if (it == haystack.end()) {
    return haystack.insert(it, std::make_pair(needle, new P(needle)))->second;
  } else {
    return it->second;
  }
}
}  // end of namespace imp
}  // end of namespace Nan

#if NODE_MODULE_VERSION > NODE_0_10_MODULE_VERSION
# include "nan_callbacks_12_inl.h"  // NOLINT(build/include)
#else
# include "nan_callbacks_pre_12_inl.h"  // NOLINT(build/include)
#endif

#endif  // NAN_CALLBACKS_H_
