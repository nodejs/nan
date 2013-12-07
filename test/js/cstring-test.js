const test     = require('tap').test
    , bindings = require('bindings');

test('cstring', function (t) {
  t.plan(2);
  t.type(bindings('cstring').r, 'function');
  t.equal(bindings('cstring').r('an utf8 strïng'), 'an utf8 strïng');
});
