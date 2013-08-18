const test     = require('tap').test
    , bindings = require('bindings');

test('returnemptystring', function (t) {
  t.plan(3);
  t.type(bindings('returnemptystring').r, 'function');
  t.equal(bindings('returnemptystring').r('a string value'), '');
  t.equal(bindings('returnemptystring').r(), '');
});
