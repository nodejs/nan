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
  JSON() {
#if (NAN_JSON_H_NEED_PARSE + NAN_JSON_H_NEED_STRINGIFY)
    Nan::MaybeLocal<v8::Value> maybeGlobalJSON = Nan::Get(
      Nan::GetCurrentContext()->Global(),
      Nan::New("JSON").ToLocalChecked()
    );

    if (!maybeGlobalJSON.IsEmpty()) {
      v8::Local<v8::Value> valGlobalJSON = maybeGlobalJSON.ToLocalChecked();

      if (valGlobalJSON->IsObject()) {
        Nan::MaybeLocal<v8::Object> maybeObjGlobalJSON =
          Nan::To<v8::Object>(valGlobalJSON);

        if (!maybeObjGlobalJSON.IsEmpty()) {
          v8::Local<v8::Object> globalJSON =
            maybeObjGlobalJSON.ToLocalChecked();

#if NAN_JSON_H_NEED_PARSE
          Nan::MaybeLocal<v8::Value> maybeParseMethod = Nan::Get(
            globalJSON, Nan::New("parse").ToLocalChecked()
          );

          if (!maybeParseMethod.IsEmpty()) {
            v8::Local<v8::Value> parseMethod =
              maybeParseMethod.ToLocalChecked();

            if (parseMethod->IsFunction()) {
              m_cb_parse.Reset(v8::Local<v8::Function>::Cast(parseMethod));
            }
          }
#endif

#if NAN_JSON_H_NEED_STRINGIFY
          Nan::MaybeLocal<v8::Value> maybeStringifyMethod = Nan::Get(
            globalJSON, Nan::New("stringify").ToLocalChecked()
          );

          if (!maybeStringifyMethod.IsEmpty()) {
            v8::Local<v8::Value> stringifyMethod =
              maybeStringifyMethod.ToLocalChecked();

            if (stringifyMethod->IsFunction()) {
              m_cb_stringify.Reset(
                v8::Local<v8::Function>::Cast(stringifyMethod)
              );
            }
          }
#endif
        }
      }
    }
#endif
  }

  ~JSON() {
#if NAN_JSON_H_NEED_PARSE
    m_cb_parse.Reset();
#endif
#if NAN_JSON_H_NEED_STRINGIFY
    m_cb_stringify.Reset();
#endif
  }

  inline
  Nan::MaybeLocal<v8::Value> Parse(v8::Local<v8::String> json_string) {
    Nan::EscapableHandleScope scope;
#if NAN_JSON_H_NEED_PARSE
    return scope.Escape(parse(json_string));
#else
#if (NODE_MAJOR_VERSION >= 7)
    Nan::MaybeLocal<v8::Value> result =
      v8::JSON::Parse(Nan::GetCurrentContext(), json_string);

    if (result.IsEmpty()) return v8::Local<v8::Value>();
    return scope.Escape(result.ToLocalChecked());
#else
    return scope.Escape(v8::JSON::Parse(json_string));
#endif
#endif
  }

  inline
  Nan::MaybeLocal<v8::String> Stringify(v8::Local<v8::Object> json_object) {
    Nan::EscapableHandleScope scope;
    Nan::MaybeLocal<v8::String> result =
#if NAN_JSON_H_NEED_STRINGIFY
      Nan::To<v8::String>(stringify(json_object));
#else
      v8::JSON::Stringify(Nan::GetCurrentContext(), json_object);
#endif
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
#endif
    if (result.IsEmpty()) return v8::Local<v8::String>();
    return scope.Escape(result.ToLocalChecked());
  }

 private:
  NAN_DISALLOW_ASSIGN_COPY_MOVE(JSON)
#if NAN_JSON_H_NEED_PARSE
  Nan::Callback m_cb_parse;
#endif
#if NAN_JSON_H_NEED_STRINGIFY
  Nan::Callback m_cb_stringify;
#endif

#if NAN_JSON_H_NEED_PARSE
  inline v8::Local<v8::Value> parse(v8::Local<v8::Value> arg) {
    if (m_cb_parse.IsEmpty()) return Nan::Undefined();
    return m_cb_parse.Call(1, &arg);
  }
#endif

#if NAN_JSON_H_NEED_STRINGIFY
  inline v8::Local<v8::Value> stringify(v8::Local<v8::Value> arg) {
    if (m_cb_stringify.IsEmpty()) return Nan::Undefined();
    return m_cb_stringify.Call(1, &arg);
  }

  inline v8::Local<v8::Value> stringify(v8::Local<v8::Value> arg,
    v8::Local<v8::String> gap) {
    if (m_cb_stringify.IsEmpty()) return Nan::Undefined();

    v8::Local<v8::Value> argv[] = {
      arg,
      Nan::Null(),
      gap
    };
    return m_cb_stringify.Call(3, argv);
  }
#endif
};

#endif /* NAN_JSON_H_ */
