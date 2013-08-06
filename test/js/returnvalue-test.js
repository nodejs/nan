const test     = require('tap').test
    , bindings = require('bindings');

test('returnvalue', function (t) {
  t.plan(3);
  t.type(bindings('returnvalue').r, 'function');
  t.equal(bindings('returnvalue').r('a string value'), 'a string value');
  t.equal(bindings('returnvalue').r(), 'default');
});
