// toys used in testing
// included first, so windows actually defines M_PI
#ifdef _WIN32
# define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <time.h>

#include <nan.h>
#include "nan_new.hpp"

namespace {

using namespace v8;

template <typename T, typename U>
struct is_same {
  static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
  static const bool value = true;
};

template <typename T, typename U>
bool
assertType(U value) {
  return is_same<v8::Local<T>, U>::value;
}

bool
stringMatches(Local<Value> value, const char * match) {
  String::Utf8Value v(value);
  return std::string(*v) == std::string(match);
}

#define _(e) NAN_TEST_EXPRESSION(e)

NAN_METHOD(testArray) {
  NanScope();
  NanTap t(args[0]);

  t.plan(3);

  t.ok(_( NanNew2<Array>()->Length() == 0));
  t.ok(_( NanNew2<Array>(7)->Length() == 7));
  t.ok(_( assertType<Array>(NanNew2<Array>(7))));

  return_NanUndefined();
}

NAN_METHOD(testBoolean) {
  NanScope();
  NanTap t(args[0]);

  t.plan(6);

  t.ok(_( NanNew2<Boolean>(true)->Value() == true));
  t.ok(_( NanNew2<Boolean>(false)->Value() == false));
  t.ok(_( assertType<Boolean>( NanNew2<Boolean>(true))));

  t.ok(_( NanNew2(true)->Value() == true));
  t.ok(_( NanNew2(false)->Value() == false));
  t.ok(_( assertType<Boolean>( NanNew2(true))));

  return_NanUndefined();
}

#if (NODE_MODULE_VERSION < 12)
# define V(x) x->BooleanValue() 
#else
# define V(x) x->ValueOf() 
#endif
NAN_METHOD(testBooleanObject) {
  NanScope();
  NanTap t(args[0]);

  t.plan(3);

  t.ok(_( assertType<BooleanObject>( NanNew2<BooleanObject>(true))));
  t.ok(_( V(NanNew2<BooleanObject>(true)) == true));
  t.ok(_( V(NanNew2<BooleanObject>(false)) == false));

  return_NanUndefined();
}
#undef V

NAN_METHOD(testDate) {
  NanScope();
  NanTap t(args[0]);

  t.plan(1);

  t.ok(_( assertType<Date>( NanNew2<Date>(double(time(NULL))))));

  return_NanUndefined();
}

int ttt = 23;

NAN_METHOD(testExternal) {
  NanScope();
  NanTap t(args[0]);

  t.plan(2);

  t.ok(_(NanNew2<External>(&ttt)->Value() == &ttt));
  t.ok(_( assertType<External>(NanNew2<External>(&ttt))));

  return_NanUndefined();
}

NAN_METHOD(testFunctionTemplate) {
  NanScope();
  NanTap t(args[0]);

  t.plan(3);

  t.ok(_( assertType<FunctionTemplate>(
          NanNew2<FunctionTemplate>(testFunctionTemplate))));
  v8::Local<String> data = NanNew2("plonk"); 
  t.ok(_( assertType<FunctionTemplate>(
          NanNew2<FunctionTemplate>( testFunctionTemplate, data))));
  v8::Local<Signature> signature = NanNew2<Signature>(); 
  t.ok(_( assertType<FunctionTemplate>(
          NanNew2<FunctionTemplate>( testFunctionTemplate, data, signature))));

  return_NanUndefined();
}

const double epsilon = 1e-9;

NAN_METHOD(testNumber) {
  NanScope();
  NanTap t(args[0]);

  t.plan(17);

  t.ok(_( NanNew2<Int32>(5)->Value() == 5 ));
  t.ok(_( NanNew2<Int32>(-42)->Value() == -42 ));
  t.ok(_( assertType<Int32>( NanNew2<Int32>(23) )));

  t.ok(_( NanNew2<Uint32>(5u)->Value() == 5u ));
  t.ok(_( assertType<Uint32>( NanNew2<Uint32>(23u) )));

  t.ok(_( NanNew2<Integer>(5)->Value() == 5 ));
  t.ok(_( NanNew2<Integer>(-1337)->Value() == -1337 ));
  t.ok(_( assertType<Integer>( NanNew2<Integer>(-42) )));

  t.ok(_( fabs(NanNew2<Number>(M_PI)->Value() - M_PI) < epsilon));
  t.ok(_( fabs(NanNew2<Number>(-M_PI)->Value() + M_PI) < epsilon));
  t.ok(_( assertType<Number>( NanNew2<Number>(M_E) )));

  //=== Convenience

  t.ok(_( NanNew2(5)->Value() == 5 ));
  t.ok(_( assertType<Int32>( NanNew2(23) )));

  t.ok(_( NanNew2(5u)->Value() == 5u ));
  t.ok(_( assertType<Uint32>( NanNew2(23u) )));

  t.ok(_( fabs(NanNew2(M_PI)->Value() - M_PI) < epsilon));
  t.ok(_( assertType<Number>( NanNew2(M_E) )));

  return_NanUndefined();
}

NAN_METHOD(testNumberObject) {
  NanScope();
  NanTap t(args[0]);

  t.plan(2);

  t.ok(_( assertType<NumberObject>( NanNew2<NumberObject>(M_PI))));
  t.ok(_( fabs(NanNew2<NumberObject>(M_PI)->NumberValue() - M_PI) < epsilon));

  return_NanUndefined();
}

NAN_METHOD(testScript) {
  NanScope();
  NanTap t(args[0]);

  t.plan(4);

  t.ok(_( assertType<Script>( NanNew2<Script>(NanNew2("2 + 3")))));
  t.ok(_( assertType<NanUnboundScript>( NanNew2<NanUnboundScript>(NanNew2("2 + 3")))));

  // for the fans of the bound script
  t.ok(_( NanRunScript( NanNew2<NanBoundScript>(NanNew2("2 + 3")))->Int32Value() == 5));
  t.ok(_( NanRunScript( NanNew2<NanUnboundScript>(NanNew2("2 + 3")))->Int32Value() == 5));

  return_NanUndefined();
}

NAN_METHOD(testSignature) {
  NanScope();
  NanTap t(args[0]);

  t.plan(3);

  typedef v8::Handle<v8::FunctionTemplate> FTH;
  t.ok(_( assertType<Signature>( NanNew2<Signature>())));
  t.ok(_( assertType<Signature>(
          NanNew2<Signature>(NanNew2<FunctionTemplate>(testSignature)))));

  Local<FunctionTemplate> arg = NanNew2<FunctionTemplate>(testSignature);
  t.ok(_( assertType<Signature>(
          NanNew2<Signature>( NanNew2<FunctionTemplate>(testSignature)
                            , 1
                            , &arg))));

  return_NanUndefined();
}

NAN_METHOD(testString) {
  NanScope();
  NanTap t(args[0]);

  t.plan(8);

  t.ok(_( stringMatches( NanNew2<String>("Hello World"), "Hello World")));
  t.ok(_( stringMatches( NanNew2<String>("Hello World", 4), "Hell")));
  t.ok(_( stringMatches( NanNew2<String>(std::string("foo")), "foo")));
  t.ok(_( assertType<String>( NanNew2<String>("plonk."))));

  //=== Convenience

  t.ok(_( stringMatches( NanNew2("using namespace nan; // is poetry"),
          "using namespace nan; // is poetry")));
  t.ok(_( assertType<String>( NanNew2("plonk."))));

  t.ok(_( stringMatches( NanNew2(std::string("bar")), "bar")));
  t.ok(_( assertType<String>( NanNew2(std::string("plonk.")))));

  return_NanUndefined();
}

#if (NODE_MODULE_VERSION < 12)
# define V(x) x->StringValue() 
#else
# define V(x) x->ValueOf() 
#endif
NAN_METHOD(testStringObject) {
  NanScope();
  NanTap t(args[0]);

  t.plan(2);

  t.ok(_( stringMatches(
          V(NanNew2<StringObject>(NanNew2<String>("plonk"))),
          "plonk")));
  t.ok(_( assertType<StringObject>(
          NanNew2<StringObject>(NanNew2<String>("plonk")))));

  return_NanUndefined();
}
#undef V

template <typename T> Handle<T> asHandle(Local<T> l) { return l; }
NAN_METHOD(testHandles) {
  NanScope();
  NanTap t(args[0]);

  t.plan(2);

  t.ok(_( assertType<String>( NanNew2( asHandle(NanNew2("foo"))))));
  t.ok(_( assertType<Uint32>( NanNew2( asHandle(NanNew2(5u))))));

  return_NanUndefined();
}

NAN_METHOD(testPersistents) {
  NanScope();
  NanTap t(args[0]);

  t.plan(1);

  Persistent<String> p;
  NanAssignPersistent(p, NanNew2("foo"));
  t.ok(_( assertType<String>( NanNew2(p))));
  NanDisposePersistent(p);

  return_NanUndefined();
}

//==============================================================================
// Regression Tests
//==============================================================================


// See https://github.com/rvagg/nan/issues/212
NAN_METHOD(testRegression212) {
  NanScope();
  NanTap t(args[0]);

  t.plan(1);

  typedef int  gint;
  typedef gint gboolean;
  t.ok(_( assertType<Boolean>( NanNew2<Boolean>(gboolean(23)))));

  return_NanUndefined();
}


//==============================================================================
// JavaScript Tests
//==============================================================================

NAN_METHOD(newIntegerWithValue) {
  NanScope();
  return_NanValue(NanNew2<Integer>(args[0]->Int32Value()));
}

NAN_METHOD(newNumberWithValue) {
  NanScope();
  return_NanValue(NanNew2<Number>(args[0]->NumberValue()));
}

NAN_METHOD(newUint32WithValue) {
  NanScope();
  return_NanValue(NanNew2<Uint32>(args[0]->Uint32Value()));
}

NAN_METHOD(newStringFromChars) {
  NanScope();
  return_NanValue(NanNew2<String>("hello?"));
}

NAN_METHOD(newStringFromCharsWithLength) {
  NanScope();
  return_NanValue(NanNew2<String>("hello?", 4));
}

NAN_METHOD(newStringFromStdString) {
  NanScope();
  return_NanValue(NanNew2<String>(std::string("hello!")));
}

NAN_METHOD(newExternal) {
  NanScope();
  return_NanValue(NanNew2<External>(&ttt));
}

void Init(Handle<Object> exports) {
  NAN_EXPORT(exports, testArray);
  NAN_EXPORT(exports, testBoolean);
  NAN_EXPORT(exports, testBooleanObject);
  NAN_EXPORT(exports, testDate);
  NAN_EXPORT(exports, testExternal);
  NAN_EXPORT(exports, testFunctionTemplate);
  NAN_EXPORT(exports, testNumber);
  NAN_EXPORT(exports, testNumberObject);
  NAN_EXPORT(exports, testScript);
  NAN_EXPORT(exports, testSignature);
  NAN_EXPORT(exports, testString);
  NAN_EXPORT(exports, testStringObject);

  NAN_EXPORT(exports, testHandles);
  NAN_EXPORT(exports, testPersistents);

  NAN_EXPORT(exports, testRegression212);

  NAN_EXPORT(exports, newIntegerWithValue);
  NAN_EXPORT(exports, newNumberWithValue);
  NAN_EXPORT(exports, newUint32WithValue);
  NAN_EXPORT(exports, newStringFromChars);
  NAN_EXPORT(exports, newStringFromCharsWithLength);
  NAN_EXPORT(exports, newStringFromStdString);

  NAN_EXPORT(exports, newExternal);
}

} // end of anonymous namespace

NODE_MODULE(nan_sketch, Init)

