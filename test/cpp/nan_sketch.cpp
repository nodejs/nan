#include <nan.h>

// toys used in testing
#ifdef _WIN32
# define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <time.h>

//==============================================================================
// Implementation sketch
//==============================================================================

namespace NanIntern { // scnr

// TODO: Generalize
template <typename T> v8::Local<T> To(v8::Handle<v8::Integer> i);

template <>
v8::Local<v8::Integer>
To<v8::Integer>(v8::Handle<v8::Integer> i) { return i->ToInteger(); }

template <> 
v8::Local<v8::Int32> 
To<v8::Int32>(v8::Handle<v8::Integer> i)   { return i->ToInt32(); }

template <>
v8::Local<v8::Uint32>
To<v8::Uint32>(v8::Handle<v8::Integer> i)  { return i->ToUint32(); }

template <typename T> struct FactoryBase { typedef v8::Local<T> return_t; };

template <typename T> struct Factory;

template <>
struct Factory<v8::Array> : public FactoryBase<v8::Array> {
  static inline return_t New();
  static inline return_t New(int length);
};

template <>
struct Factory<v8::Boolean> : public FactoryBase<v8::Boolean> {
  static inline return_t New(bool value);
};

template <>
struct Factory<v8::External> : public FactoryBase<v8::External> {
  static inline return_t New(void *value);
};

template <>
struct Factory<v8::Date> : public FactoryBase<v8::Date> {
  static inline return_t New(double value);
};

template <>
struct Factory<v8::String> : public FactoryBase<v8::String> {
  static inline return_t New(const char *value);
  static inline return_t New(const char *value, int length);
  static inline return_t New(std::string const& value);
};

//=== Numeric Types ============================================================

template <>
struct Factory<v8::Number> : public FactoryBase<v8::Number> {
  static inline return_t New(double value);
};


template <typename T>
struct IntegerFactory : public FactoryBase<T>{
  typedef typename FactoryBase<T>::return_t return_t;
  static inline return_t New(int32_t value);
  static inline return_t New(uint32_t value);
};

template <>
struct Factory<v8::Integer> : public IntegerFactory<v8::Integer> {};

template <>
struct Factory<v8::Int32> : public IntegerFactory<v8::Int32> {};

template <>
struct Factory<v8::Uint32> : public IntegerFactory<v8::Uint32> {};

} // end of namespace NanIntern

#if (NODE_MODULE_VERSION < 12)
# include "nan_implementation_pre_12.inl"
#else
# include "nan_implementation_12.inl"
#endif

//=== API ======================================================================

template <typename T>
typename NanIntern::Factory<T>::return_t
NanNew2() {
  return NanIntern::Factory<T>::New();
}

template <typename T, typename A0>
typename NanIntern::Factory<T>::return_t
NanNew2(A0 arg0) {
  return NanIntern::Factory<T>::New(arg0);
}

template <typename T, typename A0, typename A1>
typename NanIntern::Factory<T>::return_t
NanNew2(A0 arg0, A1 arg1) {
  return NanIntern::Factory<T>::New(arg0, arg1);
}

void
NanExport(v8::Handle<v8::Object> target, const char * name,
    NanFunctionCallback f)
{
  target->Set(NanNew<v8::String>(name), 
      NanNew<v8::FunctionTemplate>(f)->GetFunction());
}

#define return_NanValue(v) NanReturnValue(v)
#define NAN_EXPORT(target, function) NanExport(target, #function, function)

//==============================================================================
// Sample Module
//==============================================================================

NAN_METHOD(newIntegerWithValue) {
  NanScope();
  return_NanValue(NanNew2<v8::Integer>(args[0]->Int32Value()));
}

NAN_METHOD(newNumberWithValue) {
  NanScope();
  return_NanValue(NanNew2<v8::Number>(args[0]->NumberValue()));
}

NAN_METHOD(newUint32WithValue) {
  NanScope();
  return_NanValue(NanNew2<v8::Uint32>(args[0]->Uint32Value()));
}

NAN_METHOD(newStringFromChars) {
  NanScope();
  return_NanValue(NanNew2<v8::String>("hello?"));
}

NAN_METHOD(newStringFromCharsWithLength) {
  NanScope();
  return_NanValue(NanNew2<v8::String>("hello?", 4));
}

NAN_METHOD(newStringFromStdString) {
  NanScope();
  return_NanValue(NanNew2<v8::String>(std::string("hello!")));
}

NAN_METHOD(demoDateAndNumber) {
  NanScope();
  v8::Local<v8::Value> number = NanNew<v8::Number>(M_PI);
  v8::Local<v8::Value> date   = NanNew<v8::Date>(double(time(NULL)));
  (void)number; (void)date; // unused
  NanReturnUndefined();
}

int ttt = 23;

NAN_METHOD(newExternal) {
  NanScope();
  return_NanValue(NanNew2<v8::External>(&ttt));
}
void Init(v8::Handle<v8::Object> exports) {
  NanExport(exports, "newIntegerWithValue", newIntegerWithValue);
  NAN_EXPORT(exports, newNumberWithValue);
  NAN_EXPORT(exports, newUint32WithValue);
  NAN_EXPORT(exports, newStringFromChars);
  NAN_EXPORT(exports, newStringFromCharsWithLength);
  NAN_EXPORT(exports, newStringFromStdString);
  NAN_EXPORT(exports, demoDateAndNumber);

  NAN_EXPORT(exports, newExternal);
}

NODE_MODULE(nan_sketch, Init)

