const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'parse' });

test('parse', function (t) {
  t.plan(2);
  t.type(bindings.parse, 'function');
  t.deepEqual(
    bindings.parse('{ "a": "JSON", "string": "value" }'),
    JSON.parse('{ "a": "JSON", "string": "value" }')
  );
});
