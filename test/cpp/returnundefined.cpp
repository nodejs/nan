#include <node.h>
#include "nan.h"

NAN_METHOD(ReturnUndefined) {
  NanScope();
  NanReturnUndefined();
}

void Init (v8::Handle<v8::Object> target) {
  target->Set(
      NanSymbol("r")
    , v8::FunctionTemplate::New(ReturnUndefined)->GetFunction()
  );
}

NODE_MODULE(returnundefined, Init)

