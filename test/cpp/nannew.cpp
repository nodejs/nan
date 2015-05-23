/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

// toys used in testing
// included first, so windows actually defines M_PI
#ifdef _WIN32
# define _USE_MATH_DEFINES
#endif
#include <nan.h>
#include <cmath>
#include <ctime>
#if defined(_MSC_VER)
# pragma warning( push )
# pragma warning( disable : 4530 )
# include <string>
# pragma warning( pop )
#else
# include <string>
#endif

using namespace Nan;  // NOLINT(build/namespaces)

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
  NanTap t(info[0]);

  t.plan(3);

  t.ok(_( NanNew<Array>()->Length() == 0));
  t.ok(_( NanNew<Array>(7)->Length() == 7));
  t.ok(_( assertType<Array>(NanNew<Array>(7))));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testBoolean) {
  NanTap t(info[0]);

  t.plan(6);

  t.ok(_( NanNew<Boolean>(true)->Value() == true));
  t.ok(_( NanNew<Boolean>(false)->Value() == false));
  t.ok(_( assertType<Boolean>( NanNew<Boolean>(true))));

  t.ok(_( NanNew(true)->Value() == true));
  t.ok(_( NanNew(false)->Value() == false));
  t.ok(_( assertType<Boolean>( NanNew(true))));

  info.GetReturnValue().SetUndefined();
}

#if (NODE_MODULE_VERSION < 12)
# define V(x) x->BooleanValue()
#else
# define V(x) x->ValueOf()
#endif
NAN_METHOD(testBooleanObject) {
  NanTap t(info[0]);

  t.plan(3);

  t.ok(_( assertType<BooleanObject>( NanNew<BooleanObject>(true))));
  t.ok(_( V(NanNew<BooleanObject>(true)) == true));
  t.ok(_( V(NanNew<BooleanObject>(false)) == false));

  info.GetReturnValue().SetUndefined();
}
#undef V

NAN_METHOD(testContext) {
  NanTap t(info[0]);

  t.plan(5);
  t.ok(_( assertType<Context>( NanNew<Context>())));
  ExtensionConfiguration extensions(0, NULL);
  t.ok(_( assertType<Context>( NanNew<Context>(&extensions))));
  t.ok(_( assertType<Context>(
          NanNew<Context>(static_cast<ExtensionConfiguration *>(NULL)
          , Handle<ObjectTemplate>()))));
  t.ok(_( assertType<Context>(
          NanNew<Context>(&extensions, Handle<ObjectTemplate>()))));
  t.ok(_( assertType<Context>(
          NanNew<Context>(&extensions
          , Handle<ObjectTemplate>(), Handle<Value>()))));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testDate) {
  NanTap t(info[0]);

  t.plan(1);

  t.ok(_( assertType<Date>(
      NanNew<Date>(static_cast<double>(time(NULL))).ToLocalChecked())));

  info.GetReturnValue().SetUndefined();
}

int ttt = 23;

NAN_METHOD(testExternal) {
  NanTap t(info[0]);

  t.plan(2);

  t.ok(_(NanNew<External>(&ttt)->Value() == &ttt));
  t.ok(_( assertType<External>(NanNew<External>(&ttt))));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testFunction) {
  NanTap t(info[0]);
  t.plan(2);

  t.ok(_( assertType<Function>(NanNew<Function>(testFunction))));
  v8::Local<String> data = NanNew("plonk").ToLocalChecked();
  t.ok(_( assertType<Function>(NanNew<Function>(testFunction, data))));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testFunctionTemplate) {
  NanTap t(info[0]);

  t.plan(4);

  t.ok(_( assertType<FunctionTemplate>( NanNew<FunctionTemplate>())));
  t.ok(_( assertType<FunctionTemplate>(
          NanNew<FunctionTemplate>(testFunctionTemplate))));
  v8::Local<String> data = NanNew("plonk").ToLocalChecked();
  t.ok(_( assertType<FunctionTemplate>(
          NanNew<FunctionTemplate>( testFunctionTemplate, data))));
  v8::Local<Signature> signature = NanNew<Signature>();
  t.ok(_( assertType<FunctionTemplate>(
          NanNew<FunctionTemplate>( testFunctionTemplate, data, signature))));

  info.GetReturnValue().SetUndefined();
}

const double epsilon = 1e-9;

NAN_METHOD(testNumber) {
  NanTap t(info[0]);

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

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testNumberObject) {
  NanTap t(info[0]);

  t.plan(2);

  t.ok(_( assertType<NumberObject>( NanNew<NumberObject>(M_PI))));
  t.ok(_(
      fabs(
          NanTo<double>(NanNew<NumberObject>(M_PI)).FromJust() - M_PI
      ) < epsilon));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testObject) {
  NanTap t(info[0]);

  t.plan(1);

  t.ok(_(assertType<Object>( NanNew<Object>())));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testObjectTemplate) {
  NanTap t(info[0]);

  t.plan(1);

  t.ok(_(assertType<ObjectTemplate>( NanNew<ObjectTemplate>())));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testScript) {
  NanTap t(info[0]);

  t.plan(6);

  ScriptOrigin origin(NanNew("foo").ToLocalChecked(), NanNew(5));

  t.ok(_( assertType<Script>(NanNew<Script>(
      NanNew("2 + 3").ToLocalChecked()).ToLocalChecked())));
  t.ok(_( assertType<Script>(NanNew<Script>(
      NanNew("2 + 3").ToLocalChecked(), origin).ToLocalChecked())));
  t.ok(_( assertType<NanUnboundScript>(NanNew<NanUnboundScript>(
      NanNew("2 + 3").ToLocalChecked()).ToLocalChecked())));
  t.ok(_( assertType<NanUnboundScript>(
      NanNew<NanUnboundScript>(
          NanNew("2 + 3").ToLocalChecked()
        , origin).ToLocalChecked())));

  // for the fans of the bound script
  t.ok(_( NanTo<int32_t>(NanRunScript(
      NanNew<NanBoundScript>(NanNew("2 + 3").ToLocalChecked()
    ).ToLocalChecked()).ToLocalChecked()).FromJust() == 5));
  t.ok(_( NanTo<int32_t>(NanRunScript(
      NanNew<NanUnboundScript>(NanNew("2 + 3").ToLocalChecked()
    ).ToLocalChecked()).ToLocalChecked()).FromJust() == 5));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testSignature) {
  NanTap t(info[0]);

  t.plan(3);

  typedef FunctionTemplate FT;
  typedef Signature Sig;
  t.ok(_( assertType<Sig>(NanNew<Sig>())));
  t.ok(_( assertType<Sig>(NanNew<Sig>(NanNew<FT>(testSignature)))));

  t.ok(_( assertType<Sig>(
      NanNew<Sig>(NanNew<FT>(testSignature)))));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testString) {
  NanTap t(info[0]);

  t.plan(14);

  t.ok(_( stringMatches(
      NanNew<String>("Hello World").ToLocalChecked(), "Hello World")));
  t.ok(_( stringMatches(
      NanNew<String>("Hello World", 4).ToLocalChecked(), "Hell")));
  t.ok(_( stringMatches(
      NanNew<String>(std::string("foo")).ToLocalChecked(), "foo")));
  t.ok(_( assertType<String>(
      NanNew<String>("plonk.").ToLocalChecked())));

  t.ok(_( stringMatches( NanNew<String>().ToLocalChecked(), "")));
  t.ok(_( assertType<String>( NanNew<String>().ToLocalChecked())));

  const uint8_t *ustring = reinterpret_cast<const uint8_t *>("unsigned chars");
  t.ok(_( stringMatches(
      NanNewOneByteString(ustring).ToLocalChecked(), "unsigned chars")));
  t.ok(_( stringMatches(
      NanNewOneByteString(ustring, 8).ToLocalChecked(), "unsigned")));

  // === Convenience

  t.ok(_( stringMatches(
      NanNew("using namespace nan; // is poetry").ToLocalChecked()
    , "using namespace nan; // is poetry")));
  t.ok(_( assertType<String>( NanNew("plonk.").ToLocalChecked())));

  t.ok(_( stringMatches( NanNew("Hello World", 4).ToLocalChecked(), "Hell")));
  t.ok(_( assertType<String>( NanNew("plonk.", 4).ToLocalChecked())));

  t.ok(_( stringMatches( NanNew(std::string("bar")).ToLocalChecked(), "bar")));
  t.ok(_( assertType<String>( NanNew(std::string("plonk.")).ToLocalChecked())));

  info.GetReturnValue().SetUndefined();
}

#if (NODE_MODULE_VERSION < 12)
# define V(x) x->StringValue()
#else
# define V(x) x->ValueOf()
#endif
NAN_METHOD(testStringObject) {
  NanTap t(info[0]);

  t.plan(2);

  t.ok(_( stringMatches(
          V(NanNew<StringObject>(NanNew<String>("plonk").ToLocalChecked())),
          "plonk")));
  t.ok(_( assertType<StringObject>(
          NanNew<StringObject>(NanNew<String>("plonk").ToLocalChecked()))));

  info.GetReturnValue().SetUndefined();
}
#undef V

template <typename T> Handle<T> asHandle(Local<T> l) { return l; }
NAN_METHOD(testHandles) {
  NanTap t(info[0]);

  t.plan(2);

  t.ok(_( assertType<String>(
      NanNew( asHandle(NanNew("foo").ToLocalChecked())))));
  t.ok(_( assertType<Uint32>( NanNew( asHandle(NanNew(5u))))));

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testPersistents) {
  NanTap t(info[0]);

  t.plan(1);

  NanPersistent<String> p;
  p.Reset(NanNew("foo").ToLocalChecked());
  t.ok(_( assertType<String>( NanNew(p))));
  p.Reset();

  info.GetReturnValue().SetUndefined();
}

//==============================================================================
// Regression Tests
//==============================================================================


// See https://github.com/nodejs/nan/issues/212
NAN_METHOD(testRegression212) {
  NanTap t(info[0]);

  t.plan(1);

  typedef int  gint;
  typedef gint gboolean;
#if defined(_MSC_VER)
# pragma warning( push )
# pragma warning( disable : 4800 )
#endif
  t.ok(_( assertType<Boolean>( NanNew<Boolean>(gboolean(23)))));
#if defined(_MSC_VER)
# pragma warning( pop )
#endif

  info.GetReturnValue().SetUndefined();
}

/* Compile time regression test for https://github.com/nodejs/nan/issues/242
 * In the presence of overloaded functions NaN should be able to pick the one
 * matching NanFunctionCallback.
 */
void overloaded() {}
NAN_METHOD(overloaded) {
    overloaded();  // not unused
    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(testRegression242) {
  NanTap t(info[0]);

  // These lines must *compile*. Not much to test at runtime.
  Local<FunctionTemplate> ft = NanNew<FunctionTemplate>(overloaded);
  (void)ft;  // not unused
  Local<Function> f = NanNew<Function>(overloaded);
  (void)f;  // not unused

  t.plan(1);

  t.ok(true, "compile-time regression test #242");

  info.GetReturnValue().SetUndefined();
}


//==============================================================================
// JavaScript Tests
//==============================================================================

NAN_METHOD(newIntegerWithValue) {
  info.GetReturnValue().Set(
      NanNew(static_cast<int32_t>(NanTo<int64_t>(info[0]).FromJust())));
}

NAN_METHOD(newNumberWithValue) {
  info.GetReturnValue().Set(NanNew(NanTo<double>(info[0]).FromJust()));
}

NAN_METHOD(newUint32WithValue) {
  info.GetReturnValue().Set(NanNew(NanTo<uint32_t>(info[0]).FromJust()));
}

NAN_METHOD(newStringFromChars) {
  info.GetReturnValue().Set(NanNew("hello?").ToLocalChecked());
}

NAN_METHOD(newStringFromCharsWithLength) {
  info.GetReturnValue().Set(NanNew("hello?", 4).ToLocalChecked());
}

NAN_METHOD(newStringFromStdString) {
  info.GetReturnValue().Set(NanNew(std::string("hello!")).ToLocalChecked());
}

NAN_METHOD(newExternal) {
  info.GetReturnValue().Set(NanNew<External>(&ttt));
}

void Init(Handle<Object> exports) {
  NAN_EXPORT(exports, testArray);
  NAN_EXPORT(exports, testBoolean);
  NAN_EXPORT(exports, testBooleanObject);
  NAN_EXPORT(exports, testContext);
  NAN_EXPORT(exports, testDate);
  NAN_EXPORT(exports, testExternal);
  NAN_EXPORT(exports, testFunction);
  NAN_EXPORT(exports, testFunctionTemplate);
  NAN_EXPORT(exports, testNumber);
  NAN_EXPORT(exports, testNumberObject);
  NAN_EXPORT(exports, testObject);
  NAN_EXPORT(exports, testObjectTemplate);
  NAN_EXPORT(exports, testScript);
  NAN_EXPORT(exports, testSignature);
  NAN_EXPORT(exports, testString);
  NAN_EXPORT(exports, testStringObject);

  NAN_EXPORT(exports, testHandles);
  NAN_EXPORT(exports, testPersistents);

  NAN_EXPORT(exports, testRegression212);
  NAN_EXPORT(exports, testRegression242);

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
