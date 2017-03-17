const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'parse' });

test('parse', function (t) {
  t.plan(8);
  t.type(bindings.parse, 'function');
  t.deepEqual(
    bindings.parse('{ "a": "JSON", "string": "value" }'),
    JSON.parse('{ "a": "JSON", "string": "value" }')
  );
  t.deepEqual(
    bindings.parse('[ 1, 2, 3 ]'),
    JSON.parse('[ 1, 2, 3 ]')
  );
  t.equal(
    bindings.parse('57'),
    JSON.parse('57')
  );
  t.equal(
    bindings.parse('3.14159'),
    JSON.parse('3.14159')
  );
  t.equal(
    bindings.parse('true'),
    JSON.parse('true')
  );
  t.equal(
    bindings.parse('false'),
    JSON.parse('false')
  );
  t.equal(
    bindings.parse('"some string"'),
    JSON.parse('"some string"')
  );
});
