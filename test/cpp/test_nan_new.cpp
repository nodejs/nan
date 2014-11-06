/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

// toys used in testing
// included first, so windows actually defines M_PI
#ifdef _WIN32
# define _USE_MATH_DEFINES
#endif
#include <time.h>
#include <nan.h>
#include <cmath>
#include <string>

using namespace v8;  // NOLINT(build/namespaces)

namespace {

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

  t.ok(_( NanNew<Array>()->Length() == 0));
  t.ok(_( NanNew<Array>(7)->Length() == 7));
  t.ok(_( assertType<Array>(NanNew<Array>(7))));

  return_NanUndefined();
}

NAN_METHOD(testBoolean) {
  NanScope();
  NanTap t(args[0]);

  t.plan(6);

  t.ok(_( NanNew<Boolean>(true)->Value() == true));
  t.ok(_( NanNew<Boolean>(false)->Value() == false));
  t.ok(_( assertType<Boolean>( NanNew<Boolean>(true))));

  t.ok(_( NanNew(true)->Value() == true));
  t.ok(_( NanNew(false)->Value() == false));
  t.ok(_( assertType<Boolean>( NanNew(true))));

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

  t.ok(_( assertType<BooleanObject>( NanNew<BooleanObject>(true))));
  t.ok(_( V(NanNew<BooleanObject>(true)) == true));
  t.ok(_( V(NanNew<BooleanObject>(false)) == false));

  return_NanUndefined();
}
#undef V

NAN_METHOD(testDate) {
  NanScope();
  NanTap t(args[0]);

  t.plan(1);

  t.ok(_( assertType<Date>( NanNew<Date>(static_cast<double>(time(NULL))))));

  return_NanUndefined();
}

int ttt = 23;

NAN_METHOD(testExternal) {
  NanScope();
  NanTap t(args[0]);

  t.plan(2);

  t.ok(_(NanNew<External>(&ttt)->Value() == &ttt));
  t.ok(_( assertType<External>(NanNew<External>(&ttt))));

  return_NanUndefined();
}

NAN_METHOD(testFunctionTemplate) {
  NanScope();
  NanTap t(args[0]);

  t.plan(3);

  t.ok(_( assertType<FunctionTemplate>(
          NanNew<FunctionTemplate>(testFunctionTemplate))));
  v8::Local<String> data = NanNew("plonk");
  t.ok(_( assertType<FunctionTemplate>(
          NanNew<FunctionTemplate>( testFunctionTemplate, data))));
  v8::Local<Signature> signature = NanNew<Signature>();
  t.ok(_( assertType<FunctionTemplate>(
          NanNew<FunctionTemplate>( testFunctionTemplate, data, signature))));

  return_NanUndefined();
}

const double epsilon = 1e-9;

NAN_METHOD(testNumber) {
  NanScope();
  NanTap t(args[0]);

  t.plan(17);

  t.ok(_( NanNew<Int32>(5)->Value() == 5 ));
  t.ok(_( NanNew<Int32>(-42)->Value() == -42 ));
  t.ok(_( assertType<Int32>( NanNew<Int32>(23) )));

  t.ok(_( NanNew<Uint32>(5u)->Value() == 5u ));
  t.ok(_( assertType<Uint32>( NanNew<Uint32>(23u) )));

  t.ok(_( NanNew<Integer>(5)->Value() == 5 ));
  t.ok(_( NanNew<Integer>(-1337)->Value() == -1337 ));
  t.ok(_( assertType<Integer>( NanNew<Integer>(-42) )));

  t.ok(_( fabs(NanNew<Number>(M_PI)->Value() - M_PI) < epsilon));
  t.ok(_( fabs(NanNew<Number>(-M_PI)->Value() + M_PI) < epsilon));
  t.ok(_( assertType<Number>( NanNew<Number>(M_E) )));

  // === Convenience

  t.ok(_( NanNew(5)->Value() == 5 ));
  t.ok(_( assertType<Int32>( NanNew(23) )));

  t.ok(_( NanNew(5u)->Value() == 5u ));
  t.ok(_( assertType<Uint32>( NanNew(23u) )));

  t.ok(_( fabs(NanNew(M_PI)->Value() - M_PI) < epsilon));
  t.ok(_( assertType<Number>( NanNew(M_E) )));

  return_NanUndefined();
}

NAN_METHOD(testNumberObject) {
  NanScope();
  NanTap t(args[0]);

  t.plan(2);

  t.ok(_( assertType<NumberObject>( NanNew<NumberObject>(M_PI))));
  t.ok(_( fabs(NanNew<NumberObject>(M_PI)->NumberValue() - M_PI) < epsilon));

  return_NanUndefined();
}

NAN_METHOD(testScript) {
  NanScope();
  NanTap t(args[0]);

  t.plan(6);

  ScriptOrigin origin(NanNew("foo"), NanNew(5));

  t.ok(_( assertType<Script>( NanNew<Script>(NanNew("2 + 3")))));
  t.ok(_( assertType<Script>( NanNew<Script>(NanNew("2 + 3"), origin))));
  t.ok(_( assertType<NanUnboundScript>(
      NanNew<NanUnboundScript>(NanNew("2 + 3")))));
  t.ok(_( assertType<NanUnboundScript>(
          NanNew<NanUnboundScript>(NanNew("2 + 3"), origin))));

  // for the fans of the bound script
  t.ok(_( NanRunScript(
      NanNew<NanBoundScript>(NanNew("2 + 3")))->Int32Value() == 5));
  t.ok(_( NanRunScript(
      NanNew<NanUnboundScript>(NanNew("2 + 3")))->Int32Value() == 5));

  return_NanUndefined();
}

NAN_METHOD(testSignature) {
  NanScope();
  NanTap t(args[0]);

  t.plan(3);

  typedef FunctionTemplate FT;
  typedef Signature Sig;
  t.ok(_( assertType<Sig>(NanNew<Sig>())));
  t.ok(_( assertType<Sig>(NanNew<Sig>(NanNew<FT>(testSignature)))));

  Local<FT> arg = NanNew<FT>(testSignature);
  t.ok(_( assertType<Sig>(
      NanNew<Sig>(NanNew<FT>(testSignature), 1 , &arg))));

  return_NanUndefined();
}

NAN_METHOD(testString) {
  NanScope();
  NanTap t(args[0]);

  t.plan(10);

  t.ok(_( stringMatches( NanNew<String>("Hello World"), "Hello World")));
  t.ok(_( stringMatches( NanNew<String>("Hello World", 4), "Hell")));
  t.ok(_( stringMatches( NanNew<String>(std::string("foo")), "foo")));
  t.ok(_( assertType<String>( NanNew<String>("plonk."))));

  // These should be deprecated
  const uint8_t *ustring = reinterpret_cast<const uint8_t *>("unsigned chars");
  t.ok(_( stringMatches( NanNew<String>(ustring), "unsigned chars")));
  t.ok(_( stringMatches( NanNew<String>(ustring, 8), "unsigned")));

  // === Convenience

  t.ok(_( stringMatches( NanNew("using namespace nan; // is poetry"),
          "using namespace nan; // is poetry")));
  t.ok(_( assertType<String>( NanNew("plonk."))));

  t.ok(_( stringMatches( NanNew(std::string("bar")), "bar")));
  t.ok(_( assertType<String>( NanNew(std::string("plonk.")))));

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
          V(NanNew<StringObject>(NanNew<String>("plonk"))),
          "plonk")));
  t.ok(_( assertType<StringObject>(
          NanNew<StringObject>(NanNew<String>("plonk")))));

  return_NanUndefined();
}
#undef V

template <typename T> Handle<T> asHandle(Local<T> l) { return l; }
NAN_METHOD(testHandles) {
  NanScope();
  NanTap t(args[0]);

  t.plan(2);

  t.ok(_( assertType<String>( NanNew( asHandle(NanNew("foo"))))));
  t.ok(_( assertType<Uint32>( NanNew( asHandle(NanNew(5u))))));

  return_NanUndefined();
}

NAN_METHOD(testPersistents) {
  NanScope();
  NanTap t(args[0]);

  t.plan(1);

  Persistent<String> p;
  NanAssignPersistent(p, NanNew("foo"));
  t.ok(_( assertType<String>( NanNew(p))));
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
  t.ok(_( assertType<Boolean>( NanNew<Boolean>(gboolean(23)))));

  return_NanUndefined();
}


//==============================================================================
// JavaScript Tests
//==============================================================================

NAN_METHOD(newIntegerWithValue) {
  NanScope();
  return_NanValue(NanNew<Integer>(args[0]->Int32Value()));
}

NAN_METHOD(newNumberWithValue) {
  NanScope();
  return_NanValue(NanNew<Number>(args[0]->NumberValue()));
}

NAN_METHOD(newUint32WithValue) {
  NanScope();
  return_NanValue(NanNew<Uint32>(args[0]->Uint32Value()));
}

NAN_METHOD(newStringFromChars) {
  NanScope();
  return_NanValue(NanNew<String>("hello?"));
}

NAN_METHOD(newStringFromCharsWithLength) {
  NanScope();
  return_NanValue(NanNew<String>("hello?", 4));
}

NAN_METHOD(newStringFromStdString) {
  NanScope();
  return_NanValue(NanNew<String>(std::string("hello!")));
}

NAN_METHOD(newExternal) {
  NanScope();
  return_NanValue(NanNew<External>(&ttt));
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

}  // end of anonymous namespace

NODE_MODULE(nannew, Init)
