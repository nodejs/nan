#ifndef NAN_NAN_NEW_INCLUDED
#define NAN_NAN_NEW_INCLUDED
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
struct Factory<v8::Script> : public FactoryBase<v8::Script> {
  static inline return_t New(v8::Local<v8::String> source);
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

typedef v8::Script NanUnboundScript;

# include "nan_implementation_pre_12.inl"

#else // NODE_MODULE_VERSION >= 12

typedef v8::UnboundScript NanUnboundScript;

namespace NanIntern {

template <>
struct Factory<v8::UnboundScript> : public FactoryBase<v8::UnboundScript> {
  static inline return_t New(v8::Local<v8::String> source);
};

} // end of namespace NanIntern

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

// Convenience

inline
NanIntern::Factory<v8::Boolean>::return_t
NanNew2(bool value) {
  return NanNew2<v8::Boolean>(value);
}

inline
NanIntern::Factory<v8::Int32>::return_t
NanNew2(int32_t value) {
  return NanNew2<v8::Int32>(value);
}

inline
NanIntern::Factory<v8::Uint32>::return_t
NanNew2(uint32_t value) {
  return NanNew2<v8::Uint32>(value);
}

inline
NanIntern::Factory<v8::Number>::return_t
NanNew2(double value) {
  return NanNew2<v8::Number>(value);
}

inline
NanIntern::Factory<v8::String>::return_t
NanNew2(std::string const& value) {
  return NanNew2<v8::String>(value);
}

inline
NanIntern::Factory<v8::String>::return_t
NanNew2(const char * value) {
  return NanNew2<v8::String>(value);
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


#endif // NAN_NAN_NEW_INCLUDED
