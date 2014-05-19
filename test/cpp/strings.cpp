/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <nan.h>

NAN_METHOD(ReturnString) {
  NanScope();
  Nan::Encoding enc = Nan::UTF8;
  size_t bc;
  int flags = v8::String::HINT_MANY_WRITES_EXPECTED
                     | v8::String::NO_NULL_TERMINATION;

  if (args[1]->IsUint32()) {
    enc = (Nan::Encoding) args[1]->Uint32Value();
  }

  if (args[2]->IsUint32()) {
    flags = args[2]->Uint32Value();
  }

  void *s = NanRawString(args[0], enc, &bc, NULL, 0, flags);
  if (enc == Nan::UCS2) {
    v8::Local<v8::String> retval = NanNew<v8::String>(static_cast<uint16_t *>(s)
      , (flags & v8::String::NO_NULL_TERMINATION) ? bc / 2 : -1);
    assert((bc & 1) == 0 && "UCS2 string byte count was not even");
    NanReturnValue(retval);
  } else {
    NanReturnValue(
      NanNew<v8::String>(
          static_cast<char *>(s)
        , (flags & v8::String::NO_NULL_TERMINATION) ? bc : -1)
    );
  }
}

NAN_METHOD(ReturnCString) {
  NanScope();

  size_t bc;
  char *s = NanCString(args[0], &bc);
  v8::Local<v8::String> str = NanNew<v8::String>(s);
  delete[] s;

  NanReturnValue(str);
}

NAN_METHOD(CompareCStringToBuffer) {
  NanScope();

  size_t expectedLen = node::Buffer::Length(args[1]->ToObject());
  char* expectedChars = node::Buffer::Data(args[1]->ToObject());

  size_t actualLen;
  char* actualChars = NanCString(args[0], &actualLen);

  if (actualLen != expectedLen) {
    NanThrowError(NanNew<v8::String>("actual length != expected length"));
    NanReturnUndefined();
  }

  if (actualChars[expectedLen] != '\0') {
    NanThrowError(NanNew<v8::String>("should be null-terminated"));
    NanReturnUndefined();
  }

  if (strncmp(actualChars, expectedChars, expectedLen) != 0) {
    NanThrowError(NanNew<v8::String>("actual chars != expected chars"));
    NanReturnUndefined();
  }

  NanReturnValue(NanNew<v8::Boolean>(true));
}

NAN_METHOD(CompareRawStringToBuffer) {
  NanScope();

  size_t expectedLen = node::Buffer::Length(args[1]->ToObject());
  char* expectedChars = node::Buffer::Data(args[1]->ToObject());

  size_t actualLen;
  char* decoded = static_cast<char *>(
    NanRawString(
      args[0]
    , Nan::BASE64
    , &actualLen
    , NULL
    , 0
    , v8::String::HINT_MANY_WRITES_EXPECTED
    )
  );
  char *actualChars = new char[actualLen];
  memcpy(actualChars, decoded, actualLen);
  delete[] decoded;

  if (actualLen != expectedLen) {
    NanThrowError(NanNew<v8::String>("actual length != expected length"));
    NanReturnUndefined();
  }

  /* this is silly, it could easily be a virgin, zeroed buffer we're inspecting
  if (actualChars[expectedLen] == '\0') {
    delete[] actualChars;
    NanThrowError(NanNew<v8::String>("should not be null-terminated"));
    NanReturnUndefined();
  }*/

  if (strncmp(actualChars, expectedChars, expectedLen) != 0) {
    delete[] actualChars;
    NanThrowError(NanNew<v8::String>("actual chars != expected chars"));
    NanReturnUndefined();
  }

  delete[] actualChars;

  NanReturnValue(NanNew<v8::Boolean>(true));
}

v8::Persistent<v8::FunctionTemplate> returnString_persistent;
v8::Persistent<v8::FunctionTemplate> returnCString_persistent;
v8::Persistent<v8::FunctionTemplate> compareCStringToBuffer_persistent;
v8::Persistent<v8::FunctionTemplate> compareRawStringToBuffer_persistent;

void Init (v8::Handle<v8::Object> target) {
  NanScope();

  v8::Local<v8::FunctionTemplate> returnString =
    NanNew<v8::FunctionTemplate>(ReturnString);
  NanAssignPersistent(
    returnString_persistent
  , returnString
  );
  target->Set(
      NanNew<v8::String>("returnString")
    , returnString->GetFunction()
  );
  v8::Local<v8::FunctionTemplate> returnCString =
    NanNew<v8::FunctionTemplate>(ReturnCString);
  NanAssignPersistent(
    returnCString_persistent
  , returnCString
  );
  target->Set(
      NanNew<v8::String>("returnCString")
    , returnCString->GetFunction()
  );
  v8::Local<v8::FunctionTemplate> compareCStringToBuffer =
    NanNew<v8::FunctionTemplate>(CompareCStringToBuffer);
  NanAssignPersistent(
    compareCStringToBuffer_persistent
  , compareCStringToBuffer
  );
  target->Set(
      NanNew<v8::String>("compareCStringToBuffer")
    , compareCStringToBuffer->GetFunction()
  );
  v8::Local<v8::FunctionTemplate> compareRawStringToBuffer =
    NanNew<v8::FunctionTemplate>(CompareRawStringToBuffer);
  NanAssignPersistent(
    compareRawStringToBuffer_persistent
  , compareRawStringToBuffer
  );
  target->Set(
      NanNew<v8::String>("compareRawStringToBuffer")
    , compareRawStringToBuffer->GetFunction()
  );
}

NODE_MODULE(strings, Init)
