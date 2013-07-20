#include <node.h>
#include "../../nan.h"
#include "pi_est.h"
#include "sync.h"

using namespace v8;

// Simple synchronous access to the `Estimate()` function
NAN_METHOD(CalculateSync) {
  NanScope();

  // expect a number as the first argument
  int points = args[0]->Uint32Value();
  double est = Estimate(points);

  NanReturnValue(Number::New(est));
}
