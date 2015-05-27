/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

static const size_t DATA_SIZE = 26;

static char data[DATA_SIZE];

void callback(char* data, void* hint) {
  free(data);
}

NAN_METHOD(New1) {
  char *dynamic_data = static_cast<char *>(malloc(DATA_SIZE));
  for (unsigned char i = 0; i < DATA_SIZE; i++) {
    dynamic_data[i] = 'a' + i;
  }
  info.GetReturnValue().Set(
      NanNewBuffer(dynamic_data, DATA_SIZE, callback, 0).ToLocalChecked());
}

NAN_METHOD(New2) {
  v8::Local<v8::Object> buf = NanNewBuffer(DATA_SIZE).ToLocalChecked();
  char* pbuf = node::Buffer::Data(buf);
  for (unsigned char i = 0; i < DATA_SIZE; i++) {
    pbuf[i] = 'a' + i;
  }
  info.GetReturnValue().Set(buf);
}

NAN_METHOD(New3) {
  char *dynamic_data = static_cast<char *>(malloc(DATA_SIZE));
  for (unsigned char i = 0; i < DATA_SIZE; i++) {
    dynamic_data[i] = 'a' + i;
  }
  info.GetReturnValue().Set(NanNewBuffer(dynamic_data, DATA_SIZE));
}

NAN_METHOD(Copy) {
  info.GetReturnValue().Set(NanCopyBuffer(data, DATA_SIZE).ToLocalChecked());
}


void Init (v8::Handle<v8::Object> target) {
  for (unsigned char i = 0; i < DATA_SIZE; i++) {
    data[i] = 'a' + i;
  }
  NanSet(target
    , NanNew<v8::String>("new1").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(New1)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("new2").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(New2)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("new3").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(New3)->GetFunction()
  );
  NanSet(target
    , NanNew<v8::String>("copy").ToLocalChecked()
    , NanNew<v8::FunctionTemplate>(Copy)->GetFunction()
  );
}

NODE_MODULE(buffer, Init)
