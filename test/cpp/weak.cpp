/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

/*template<typename T, typename P> void __NanMakeWeakHelper(v8::Persistent<T> &handle, P *parameters, typename v8::WeakCallbackData<T, P>::Callback callback) {
  handle.SetWeak(parameters, callback);
}*/

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
    //delete data.GetParameter();
}

/*    template<typename T, typename P>                                           \
    void _weakCallback(const  v8::WeakCallbackData<T, P> &data) {       \
        NanScope();                                                            \
         WeakCallbackData<T, P> wcbd(                                          \
             data.GetValue()                                                   \
           , data.GetParameter());                                             \
        weakCallback(wcbd);                                                            \
    }                                                                          \
                                                                               \
    template<class T, typename P> void weakCallback(const WeakCallbackData<T, P> &data) {
delete data.GetParameter();
}*/


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
    #define _NAN_COMMA() ,
// only works on < 0.11
//    NanMakeWeak(obj, parameter, weakCallback<v8::Value _NAN_COMMA() int*>);
// only works on >= 0.11.8
    NanMakeWeak(obj, parameter, weakCallback);
//    obj.SetWeak(parameter, &_weakCallback);
    //NanMakeWeakHelper(obj, parameter, &_weakCallback);
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
