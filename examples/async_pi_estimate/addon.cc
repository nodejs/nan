#include <node.h>
#include "../../nan.h"
#include "sync.h"
#include "async.h"

using namespace v8;

// Expose synchronous and asynchronous access to our
// Estimate() function
void InitAll(Handle<Object> exports) {
  exports->Set(NanSymbol("calculateSync"),
    FunctionTemplate::New(CalculateSync)->GetFunction());

  exports->Set(NanSymbol("calculateAsync"),
    FunctionTemplate::New(CalculateAsync)->GetFunction());
}

NODE_MODULE(addon, InitAll)
