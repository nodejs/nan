const test     = require('tap').test
    , bindings = require('bindings');

test('returnundefined', function (t) {
  t.plan(3);
  t.type(bindings('returnundefined').r, 'function');
  t.equal(bindings('returnundefined').r('a string value'), undefined);
  t.equal(bindings('returnundefined').r(), undefined);
});
