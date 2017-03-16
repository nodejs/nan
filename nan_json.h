/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2017 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_JSON_H_
#define NAN_JSON_H_

#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 12
#define NAN_JSON_H_NEED_PARSE 1
#else
#define NAN_JSON_H_NEED_PARSE 0
#endif

#if NODE_MAJOR_VERSION >= 7
#define NAN_JSON_H_NEED_STRINGIFY 0
#else
#define NAN_JSON_H_NEED_STRINGIFY 1
#endif

class JSON {
 public:
  JSON() {
#if NAN_JSON_H_NEED_PARSE + NAN_JSON_H_NEED_STRINGIFY
    Nan::MaybeLocal<v8::Value> maybe_global_json = Nan::Get(
      Nan::GetCurrentContext()->Global(),
      Nan::New("JSON").ToLocalChecked()
    );

    if (!maybe_global_json.IsEmpty()) {
      v8::Local<v8::Value> val_global_json = maybe_global_json.ToLocalChecked();

      if (val_global_json->IsObject()) {
        Nan::MaybeLocal<v8::Object> maybe_obj_global_json =
          Nan::To<v8::Object>(val_global_json);

        if (!maybe_obj_global_json.IsEmpty()) {
          v8::Local<v8::Object> global_json =
            maybe_obj_global_json.ToLocalChecked();

#if NAN_JSON_H_NEED_PARSE
          Nan::MaybeLocal<v8::Value> maybe_parse_method = Nan::Get(
            global_json, Nan::New("parse").ToLocalChecked()
          );

          if (!maybe_parse_method.IsEmpty()) {
            v8::Local<v8::Value> parse_method =
              maybe_parse_method.ToLocalChecked();

            if (parse_method->IsFunction()) {
              parse_cb_.Reset(parse_method.As<v8::Function>());
            }
          }
#endif  // NAN_JSON_H_NEED_PARSE

#if NAN_JSON_H_NEED_STRINGIFY
          Nan::MaybeLocal<v8::Value> maybe_stringify_method = Nan::Get(
            global_json, Nan::New("stringify").ToLocalChecked()
          );

          if (!maybe_stringify_method.IsEmpty()) {
            v8::Local<v8::Value> stringify_method =
              maybe_stringify_method.ToLocalChecked();

            if (stringify_method->IsFunction()) {
              stringify_cb_.Reset(stringify_method.As<v8::Function>());
            }
          }
#endif  // NAN_JSON_H_NEED_STRINGIFY
        }
      }
    }
#endif  // NAN_JSON_H_NEED_PARSE + NAN_JSON_H_NEED_STRINGIFY
  }

  inline
  Nan::MaybeLocal<v8::Value> Parse(v8::Local<v8::String> json_string) {
    Nan::EscapableHandleScope scope;
#if NAN_JSON_H_NEED_PARSE
    return scope.Escape(parse(json_string));
#else
#if NODE_MAJOR_VERSION >= 7
    Nan::MaybeLocal<v8::Value> result =
      v8::JSON::Parse(Nan::GetCurrentContext(), json_string);

    if (result.IsEmpty()) return v8::Local<v8::Value>();
    return scope.Escape(result.ToLocalChecked());
#else
    return scope.Escape(v8::JSON::Parse(json_string));
#endif  // NODE_MAJOR_VERSION >= 7
#endif  // NAN_JSON_H_NEED_PARSE
  }

  inline
  Nan::MaybeLocal<v8::String> Stringify(v8::Local<v8::Object> json_object) {
    Nan::EscapableHandleScope scope;
    Nan::MaybeLocal<v8::String> result =
#if NAN_JSON_H_NEED_STRINGIFY
      Nan::To<v8::String>(stringify(json_object));
#else
      v8::JSON::Stringify(Nan::GetCurrentContext(), json_object);
#endif  // NAN_JSON_H_NEED_STRINGIFY
    if (result.IsEmpty()) return v8::Local<v8::String>();
    return scope.Escape(result.ToLocalChecked());
  }

  inline
  Nan::MaybeLocal<v8::String> Stringify(v8::Local<v8::Object> json_object,
    v8::Local<v8::String> gap) {
    Nan::EscapableHandleScope scope;
    Nan::MaybeLocal<v8::String> result =
#if NAN_JSON_H_NEED_STRINGIFY
      Nan::To<v8::String>(stringify(json_object, gap));
#else
      v8::JSON::Stringify(Nan::GetCurrentContext(), json_object, gap);
#endif  // NAN_JSON_H_NEED_STRINGIFY
    if (result.IsEmpty()) return v8::Local<v8::String>();
    return scope.Escape(result.ToLocalChecked());
  }

 private:
  NAN_DISALLOW_ASSIGN_COPY_MOVE(JSON)
#if NAN_JSON_H_NEED_PARSE
  Nan::Callback parse_cb_;
#endif  // NAN_JSON_H_NEED_PARSE
#if NAN_JSON_H_NEED_STRINGIFY
  Nan::Callback stringify_cb_;
#endif  // NAN_JSON_H_NEED_STRINGIFY

#if NAN_JSON_H_NEED_PARSE
  inline v8::Local<v8::Value> parse(v8::Local<v8::Value> arg) {
    if (parse_cb_.IsEmpty()) return Nan::Undefined();
    return parse_cb_.Call(1, &arg);
  }
#endif  // NAN_JSON_H_NEED_PARSE

#if NAN_JSON_H_NEED_STRINGIFY
  inline v8::Local<v8::Value> stringify(v8::Local<v8::Value> arg) {
    if (stringify_cb_.IsEmpty()) return Nan::Undefined();
    return stringify_cb_.Call(1, &arg);
  }

  inline v8::Local<v8::Value> stringify(v8::Local<v8::Value> arg,
    v8::Local<v8::String> gap) {
    if (stringify_cb_.IsEmpty()) return Nan::Undefined();

    v8::Local<v8::Value> argv[] = {
      arg,
      Nan::Null(),
      gap
    };
    return stringify_cb_.Call(3, argv);
  }
#endif  // NAN_JSON_H_NEED_STRINGIFY
};

#endif  // NAN_JSON_H_
