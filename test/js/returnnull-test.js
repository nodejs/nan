const test     = require('tap').test
    , bindings = require('bindings');

test('returnnull', function (t) {
  t.plan(3);
  t.type(bindings('returnnull').r, 'function');
  t.equal(bindings('returnnull').r('a string value'), null);
  t.equal(bindings('returnnull').r(), null);
});
