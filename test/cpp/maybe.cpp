/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2019 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <nan.h>

using namespace Nan;  // NOLINT(build/namespaces)

NAN_METHOD(Test) {
  info.GetReturnValue().Set(false);
  {
    Maybe<int> mb = Nothing<int>();
    if (mb.IsJust()) return;
    if (!mb.IsNothing()) return;
    if (mb.To(NULL)) return;
  }
  {
    Maybe<int> mb = Just(42);
    if (!mb.IsJust()) return;
    if (mb.IsNothing()) return;
    if (42 != mb.FromJust()) return;
    if (42 != mb.ToChecked()) return;
    mb.Check();
    int v;
    if (!mb.To(&v)) return;
    if (42 != v) return;
  }
  info.GetReturnValue().Set(true);
}

NAN_MODULE_INIT(Init) {
  SetMethod(target, "test", Test);
}

NODE_MODULE(maybe, Init)
