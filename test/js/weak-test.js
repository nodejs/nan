const test     = require('tap').test
    , bindings = require('bindings');

test('weak', function (t) {
  t.plan(2);

  var weak = bindings('weak');
  t.type(weak.hustle, 'function');
  t.equal(weak.hustle(), 'result');
});
