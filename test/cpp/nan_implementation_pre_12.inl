
//==============================================================================
// node v0.10 Implementation
//==============================================================================


namespace NanIntern {

//=== Array ====================================================================

Factory<v8::Array>::return_t
Factory<v8::Array>::New() {
  return v8::Array::New();
}

Factory<v8::Array>::return_t
Factory<v8::Array>::New(int length) {
  return v8::Array::New(length);
}

//=== Boolean ==================================================================

Factory<v8::Boolean>::return_t
Factory<v8::Boolean>::New(bool value) {
  return v8::Boolean::New(value)->ToBoolean();
}

//=== External =================================================================

Factory<v8::External>::return_t
Factory<v8::External>::New(void * value) {
  return v8::External::New(value);
}

//=== Date =====================================================================

Factory<v8::Date>::return_t
Factory<v8::Date>::New(double value) {
  return v8::Date::New(value).As<v8::Date>();
}

//=== String ===================================================================

Factory<v8::String>::return_t
Factory<v8::String>::New(const char * value) {
  return v8::String::New(value);
}

Factory<v8::String>::return_t
Factory<v8::String>::New(const char * value, int length) {
  return v8::String::New(value, length);
}

Factory<v8::String>::return_t
Factory<v8::String>::New(std::string const& value) {
  return v8::String::New( &*value.begin(), value.size());
}

//=== Number ===================================================================

Factory<v8::Number>::return_t
Factory<v8::Number>::New(double value) {
  return v8::Number::New(value);
}

//=== Integer, Int32 and Uint32 ================================================

template <typename T>
typename IntegerFactory<T>::return_t
IntegerFactory<T>::New(int32_t value) {
    return To<T>(T::New(value));
}

template <typename T>
typename IntegerFactory<T>::return_t
IntegerFactory<T>::New(uint32_t value) {
    return To<T>(T::NewFromUnsigned(value));
}

} // end of namespace NanIntern
