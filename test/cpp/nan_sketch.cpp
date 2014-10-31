// toys used in testing
// included first, so windows actually defines M_PI
#ifdef _WIN32
# define _USE_MATH_DEFINES
#endif
#include <cmath>


#include <nan.h>

#include "nan_new.hpp"

#include <time.h>

using namespace v8;

struct NanTap {
  NanTap(Handle<Object> t) : t_() {
    NanAssignPersistent(t_, t);
  };

  void plan(int i) {
    NanScope();
    v8::Handle<Value> arg = NanNew2(i);
    NanMakeCallback(NanNew(t_), "plan", 1, &arg);
  }

  void ok(bool isOk, const char * msg = NULL) {
    NanScope();
    v8::Handle<Value> args[2];
    args[0] = NanNew2(isOk);
    if (msg) args[1] = NanNew2(msg);
    NanMakeCallback(NanNew(t_), "ok", msg ? 2 : 1, args);
  }

  private:
  
  Persistent<Object> t_;
};

#define NAN_TEST_EXPRESSION(expression) ( expression ), "C++: '" #expression "' is false"

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

#define _(e) NAN_TEST_EXPRESSION(e)

NAN_METHOD(testNumbers) {
  NanScope();
  NanTap t(args[0]->ToObject());

  t.plan(14);

  t.ok(_( NanNew2<Int32>(5)->Value() == 5 ));
  t.ok(_( assertType<Int32>( NanNew2<Int32>(23) )));

  t.ok(_( NanNew2<Uint32>(5u)->Value() == 5u ));
  t.ok(_( assertType<Uint32>( NanNew2<Uint32>(23u) )));

  t.ok(_( NanNew2<Integer>(5)->Value() == 5 ));
  t.ok(_( assertType<Integer>( NanNew2<Integer>(23) )));

  t.ok(_( NanNew2<Number>(M_PI)->Value() == M_PI ));
  t.ok(_( assertType<Number>( NanNew2<Number>(M_E) )));

  //=== Convenience ============================================================

  t.ok(_( NanNew2(5)->Value() == 5 ));
  t.ok(_( assertType<Int32>( NanNew2(23) )));

  t.ok(_( NanNew2(5u)->Value() == 5u ));
  t.ok(_( assertType<Uint32>( NanNew2(23u) )));

  t.ok(_( NanNew2(M_PI)->Value() == M_PI ));
  t.ok(_( assertType<Number>( NanNew2(M_E) )));
  
  NanReturnUndefined();
}

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

NAN_METHOD(demoDateAndNumber) {
  NanScope();
  Local<Value> number = NanNew<Number>(M_PI);
  Local<Value> date   = NanNew<Date>(double(time(NULL)));
  (void)number; (void)date; // unused
  NanReturnUndefined();
}

int ttt = 23;

NAN_METHOD(newExternal) {
  NanScope();
  return_NanValue(NanNew2<External>(&ttt));
}
void Init(Handle<Object> exports) {
  NanExport(exports, "newIntegerWithValue", newIntegerWithValue);
  NAN_EXPORT(exports, newNumberWithValue);
  NAN_EXPORT(exports, newUint32WithValue);
  NAN_EXPORT(exports, newStringFromChars);
  NAN_EXPORT(exports, newStringFromCharsWithLength);
  NAN_EXPORT(exports, newStringFromStdString);
  NAN_EXPORT(exports, demoDateAndNumber);

  NAN_EXPORT(exports, newExternal);

  NAN_EXPORT(exports, testNumbers);
}

NODE_MODULE(nan_sketch, Init)

