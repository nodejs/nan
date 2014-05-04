const test     = require('tap').test
    , bindings = require('bindings');

test('weak', function (t) {
  t.plan(3);

  var weak = bindings('weak');
  t.type(weak.hustle, 'function');

  function f() {
    var count = 0;
    weak.hustle(function () {
      t.ok(count++ < 2);
    });
  };

  f();

  // run weak callback, should not dispose
  gc();

  // run weak callback, should dispose
  gc();

  // do not run weak callback
  gc();
});
