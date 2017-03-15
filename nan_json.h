/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2017 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_JSON_H_
#define NAN_JSON_H_

#if ((NODE_MAJOR_VERSION == 0) && (NODE_MINOR_VERSION < 12))
#define NAN_JSON_H_NEED_PARSE 1
#else
#define NAN_JSON_H_NEED_PARSE 0
#endif

#if (NODE_MAJOR_VERSION >= 7)
#define NAN_JSON_H_NEED_STRINGIFY 0
#else
#define NAN_JSON_H_NEED_STRINGIFY 1
#endif

class JSON {
 public:
  static inline
  Nan::MaybeLocal<v8::Value> Parse(v8::Local<v8::String> jsonString) {
    Nan::HandleScope scope;
#if NAN_JSON_H_NEED_PARSE
    return parse(jsonString);
#else
#if (NODE_MAJOR_VERSION >= 7)
    return v8::JSON::Parse(Nan::GetCurrentContext(), jsonString);
#else
    return v8::JSON::Parse(jsonString);
#endif
#endif
  }

  static inline
  Nan::MaybeLocal<v8::String> Stringify(v8::Local<v8::Object> jsonObject) {
    Nan::HandleScope scope;
#if NAN_JSON_H_NEED_STRINGIFY
    return Nan::To<v8::String>(stringify(jsonObject));
#else
    return v8::JSON::Stringify(Nan::GetCurrentContext(), jsonObject);
#endif
  }

  static inline
  Nan::MaybeLocal<v8::String> Stringify(v8::Local<v8::Object> jsonObject,
    v8::Local<v8::String> gap) {
    Nan::HandleScope scope;
#if NAN_JSON_H_NEED_STRINGIFY
    return Nan::To<v8::String>(stringify(jsonObject, gap));
#else
    return v8::JSON::Stringify(Nan::GetCurrentContext(), jsonObject, gap);
#endif
  }

 private:
  NAN_DISALLOW_ASSIGN_COPY_MOVE(JSON)
  JSON();
  ~JSON();

  static v8::Local<v8::Value> Call(const char *method,
    int argc, v8::Local<v8::Value> *argv) {
    v8::Local<v8::Value> globalJSON =
      Nan::GetCurrentContext()->Global()->Get(
        Nan::New("JSON").ToLocalChecked()
      );

    if (!globalJSON->IsObject()) {
      return Nan::Undefined();
    }

    v8::Local<v8::Object> json = globalJSON->ToObject();

    v8::Local<v8::Value> thisMethod =
      json->Get(Nan::New(method).ToLocalChecked());

    if (thisMethod.IsEmpty() || !thisMethod->IsFunction()) {
      return Nan::Undefined();
    }

    v8::Local<v8::Function> methodFunction =
      v8::Local<v8::Function>::Cast(thisMethod);

    return methodFunction->Call(json, argc, argv);
  }

#if NAN_JSON_H_NEED_PARSE
  static inline v8::Local<v8::Value> parse(v8::Local<v8::Value> arg) {
    return Call("parse", 1, &arg);
  }
#endif

#if NAN_JSON_H_NEED_STRINGIFY
  static inline v8::Local<v8::Value> stringify(v8::Local<v8::Value> arg) {
    return Call("stringify", 1, &arg);
  }

  static inline v8::Local<v8::Value> stringify(v8::Local<v8::Value> arg,
    v8::Local<v8::String> gap) {
    v8::Local<v8::Value> argv[] = {
      arg,
      Nan::Null(),
      gap
    };
    return Call("stringify", 3, argv);
  }
#endif
};

#endif /* NAN_JSON_H_ */
