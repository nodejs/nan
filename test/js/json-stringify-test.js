const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'stringify' });

test('stringify', function (t) {
  t.plan(22);
  t.type(bindings.stringify, 'function');
  t.equal(
    bindings.stringify({ "a": "JSON", "object": "value" }),
    JSON.stringify({ "a": "JSON", "object": "value" })
  );
  t.equal(
    bindings.stringify({ "a": "JSON", "object": "value" }, null, 2),
    JSON.stringify({ "a": "JSON", "object": "value" }, null, 2)
  );
  t.equal(
    bindings.stringify({ "a": "JSON", "object": "value" }, null, '++'),
    JSON.stringify({ "a": "JSON", "object": "value" }, null, '++')
  );
  t.equal(
    bindings.stringify([ 1, 2, 3 ]),
    JSON.stringify([ 1, 2, 3 ])
  );
  t.equal(
    bindings.stringify([ 1, 2, 3 ], null, 2),
    JSON.stringify([ 1, 2, 3 ], null, 2)
  );
  t.equal(
    bindings.stringify([ 1, 2, 3 ], null, '++'),
    JSON.stringify([ 1, 2, 3 ], null, '++')
  );
  t.equal(
    bindings.stringify("a string"),
    JSON.stringify("a string")
  );
  t.equal(
    bindings.stringify("a string", null, 2),
    JSON.stringify("a string", null, 2)
  );
  t.equal(
    bindings.stringify("a string", null, '++'),
    JSON.stringify("a string", null, '++')
  );
  t.equal(
    bindings.stringify(3.14159),
    JSON.stringify(3.14159)
  );
  t.equal(
    bindings.stringify(3.14159, null, 2),
    JSON.stringify(3.14159, null, 2)
  );
  t.equal(
    bindings.stringify(3.14159, null, '++'),
    JSON.stringify(3.14159, null, '++')
  );
  t.equal(
    bindings.stringify(-31),
    JSON.stringify(-31)
  );
  t.equal(
    bindings.stringify(-31, null, 2),
    JSON.stringify(-31, null, 2)
  );
  t.equal(
    bindings.stringify(-31, null, '++'),
    JSON.stringify(-31, null, '++')
  );
  t.equal(
    bindings.stringify(true),
    JSON.stringify(true)
  );
  t.equal(
    bindings.stringify(true, null, 2),
    JSON.stringify(true, null, 2)
  );
  t.equal(
    bindings.stringify(true, null, '++'),
    JSON.stringify(true, null, '++')
  );
  t.equal(
    bindings.stringify(false),
    JSON.stringify(false)
  );
  t.equal(
    bindings.stringify(false, null, 2),
    JSON.stringify(false, null, 2)
  );
  t.equal(
    bindings.stringify(false, null, '++'),
    JSON.stringify(false, null, '++')
  );
});
