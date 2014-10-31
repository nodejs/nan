#include <nan.h>

#include "nan_new.hpp"

// toys used in testing
#ifdef _WIN32
# define _USE_MATH_DEFINES
#endif
#include <cmath>
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


#define _(e) NAN_TEST_EXPRESSION(e)

NAN_METHOD(testNumbers) {
  NanScope();
  NanTap t(args[0]->ToObject());

  t.plan(1);
  t.ok(_( NanNew2(5)->Value() == 5 ));

  
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

