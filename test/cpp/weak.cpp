/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

//NAN_WEAK_CALLBACK(weakCallback) {
//    data.GetIsolate();
//    data.GetValue();
//    data.getParameter();
/*    if (false) {
        NanMakeWeak(data.GetValue(), data.getParameter(), &weakCallback);
    } else {*/
//	delete data.GetParameter();
//    }
//}

NAN_WEAK_CALLBACK(weakCallback) {
    delete data.GetParameter();
}

//template<class T, class P> void name(const WeakCallbackData<T, P> &data);
/*template<typename T, typename P> void _weakCallback(v8::Persistent<v8::Value> object, void *data) {
    NanScope();
    WeakCallbackData<T, P> wcbd(NanPersistentToLocal(static_cast<v8::Persistent<T> >(object)), static_cast<P*>(data));
    weakCallback(wcbd);
}

template<class T, typename P> void weakCallback(const WeakCallbackData<T, P> &data) {
}*/

v8::Handle<v8::String> wrap() {
    NanScope();
    v8::Local<v8::String> lstring = v8::String::New("result");

    int *parameter = new int(3);

    NanInitPersistent(obj, lstring);
    #define COMMA() ,
    NanMakeWeak(obj, parameter, weakCallback<v8::Value COMMA() int*>);
    return lstring;
}

NAN_METHOD(Hustle) {
  NanScope();
  NanReturnValue(wrap());
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("hustle")
    , v8::FunctionTemplate::New(Hustle)->GetFunction()
  );
}

NODE_MODULE(weak, Init)
