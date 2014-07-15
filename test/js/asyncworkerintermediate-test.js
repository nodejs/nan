const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'asyncworkerintermediate' });

test('asyncworkerintermediate', function (t) {
  var worker = bindings.a;
  t.type(worker, 'function')
  worker(200, function (err, state, arg) {
    if(state == 'intermediate')
      t.equal(arg, 200);
    else
      t.end()
  })
})
