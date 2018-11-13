/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

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
      NewBuffer(dynamic_data, DATA_SIZE, callback, 0).ToLocalChecked());
}

NAN_METHOD(New2) {
  v8::Local<v8::Object> buf = NewBuffer(DATA_SIZE).ToLocalChecked();
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
  info.GetReturnValue().Set(
      NewBuffer(dynamic_data, DATA_SIZE).ToLocalChecked());
}

NAN_METHOD(Copy) {
  info.GetReturnValue().Set(CopyBuffer(data, DATA_SIZE).ToLocalChecked());
}


NAN_MODULE_INIT(Init) {
  for (unsigned char i = 0; i < DATA_SIZE; i++) {
    data[i] = 'a' + i;
  }
  Set(target
    , New<v8::String>("new1").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(New1)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("new2").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(New2)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("new3").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(New3)).ToLocalChecked()
  );
  Set(target
    , New<v8::String>("copy").ToLocalChecked()
    , GetFunction(New<v8::FunctionTemplate>(Copy)).ToLocalChecked()
  );
}

NODE_MODULE(buffer, Init)
