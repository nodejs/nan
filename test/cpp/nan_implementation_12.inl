
//==============================================================================
// v0.11 Implementation
//==============================================================================


namespace NanIntern {

//=== Array ====================================================================

Factory<v8::Array>::return_t
Factory<v8::Array>::New() {
  return v8::Array::New(v8::Isolate::GetCurrent());
}

Factory<v8::Array>::return_t
Factory<v8::Array>::New(int length) {
  return v8::Array::New(v8::Isolate::GetCurrent(), length);
}

//=== Boolean ==================================================================

Factory<v8::Boolean>::return_t
Factory<v8::Boolean>::New(bool value) {
  return v8::Boolean::New(v8::Isolate::GetCurrent(), value);
}

//=== External =================================================================

Factory<v8::External>::return_t
Factory<v8::External>::New(void * value) {
  return v8::External::New(v8::Isolate::GetCurrent(), value);
}

//=== Date =====================================================================

Factory<v8::Date>::return_t
Factory<v8::Date>::New(double value) {
  return v8::Date::New(v8::Isolate::GetCurrent(), value).As<v8::Date>();
}

//=== String ===================================================================

Factory<v8::String>::return_t
Factory<v8::String>::New(const char * value) {
  return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), value);
}

Factory<v8::String>::return_t
Factory<v8::String>::New(const char * value, int length) {
  return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), value,
      v8::String::kNormalString, length);
}

Factory<v8::String>::return_t
Factory<v8::String>::New(std::string const& value) {
  return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(),
      &*value.begin(), v8::String::kNormalString, value.size());
}

//=== Number ===================================================================

Factory<v8::Number>::return_t
Factory<v8::Number>::New(double value) {
  return v8::Number::New(v8::Isolate::GetCurrent(), value);
}

//=== Integer, Int32 and Uint32 ================================================

template <typename T>
typename IntegerFactory<T>::return_t
IntegerFactory<T>::New(int32_t value) {
    return To<T>(T::New(v8::Isolate::GetCurrent(), value));
}

template <typename T>
typename IntegerFactory<T>::return_t
IntegerFactory<T>::New(uint32_t value) {
    return To<T>(T::NewFromUnsigned(v8::Isolate::GetCurrent(), value));
}

} // end of namespace NanIntern
