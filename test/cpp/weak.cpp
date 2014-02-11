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
