const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'symbols' });

test('symbols', function (t) {
  t.plan(2);
  t.type(bindings.key, 'string');
  t.equal(bindings.key, 'a property');
});
