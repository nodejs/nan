/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(HasPrivateYes) {
  v8::Local<v8::Object> object = New<v8::Object>();
  v8::Local<v8::String> key = New("key").ToLocalChecked();
  v8::Local<v8::String> value = New("value").ToLocalChecked();
  Maybe<bool> mb = SetPrivate(object, key, value);
  bool v1 = mb.IsJust() ? mb.FromJust() : false;
  mb = HasPrivate(object, key);
  bool v2 = mb.IsJust() ? mb.FromJust() : false;
  MaybeLocal<v8::Value> mv = GetPrivate(object, key);
  bool v3 = mv.IsEmpty() ? false :
      !strcmp(*Utf8String(mv.ToLocalChecked()), "value");
  info.GetReturnValue().Set(v1 && v2 && v3);
}

NAN_METHOD(HasPrivateNo) {
  v8::Local<v8::Object> object = New<v8::Object>();
  Maybe<bool> mb = HasPrivate(object, New("key").ToLocalChecked());
  bool v1 = mb.IsJust() ? !mb.FromJust() : false;
  MaybeLocal<v8::Value> q = GetPrivate(object, New("key").ToLocalChecked());
  bool v2 = !q.IsEmpty() ? q.ToLocalChecked()->IsUndefined() : false;
  info.GetReturnValue().Set(v1 && v2);
}

NAN_METHOD(DeletePrivateNo) {
  v8::Local<v8::Object> object = New<v8::Object>();
  v8::Local<v8::String> key = New("key").ToLocalChecked();
  v8::Local<v8::String> value = New("value").ToLocalChecked();
  Maybe<bool> mb = DeletePrivate(object, key);
  bool v1 = mb.IsJust() ? mb.FromJust() : false;
  mb = SetPrivate(object, key, value);
  bool v2 = mb.IsJust() ? mb.FromJust() : false;
  mb = DeletePrivate(object, key);
  bool v3 = mb.IsJust() ? mb.FromJust() : false;
  info.GetReturnValue().Set(v1 && v2 && v3);
}

NAN_METHOD(NoConflict) {
  v8::Local<v8::Object> object = New<v8::Object>();
  v8::Local<v8::String> key = New("key").ToLocalChecked();
  v8::Local<v8::String> value = New("value").ToLocalChecked();
  v8::Local<v8::String> other_value = New("other_value").ToLocalChecked();
  SetPrivate(object, key, value);
  Set(object, key, other_value);
  v8::Local<v8::Value> got = GetPrivate(object, key).ToLocalChecked();
  bool v1 = To<v8::String>(got).ToLocalChecked()->StrictEquals(value);
  v8::Local<v8::Value> got_other = Get(object, key).ToLocalChecked();
  bool v2 = got_other->StrictEquals(other_value);
  DeletePrivate(object, key);
  got_other = Get(object, key).ToLocalChecked();
  bool v3 = got_other->StrictEquals(other_value);
  info.GetReturnValue().Set(v1 && v2 && v3);
}

NAN_MODULE_INIT(Init) {
  Set(target
    , New<v8::String>("hasPrivateYes").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(HasPrivateYes)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("hasPrivateNo").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(HasPrivateNo)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("deletePrivateNo").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(DeletePrivateNo)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("noConflict").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(NoConflict)).ToLocalChecked()
  );
}

NODE_MODULE(private, Init)
