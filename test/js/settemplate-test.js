const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'settemplate' })
	
test('SetPrototypeTemplate', function (t) {
  t.plan(4);
  var r = new bindings.MyObject();
  t.type(r, 'object');
  r = Object.getPrototypeOf(r);
  t.type(r, 'object');
  t.type(r.prototypeProp, 'string');
  t.equal(r.prototypeProp, 'a prototype property');
});

test('SetInstanceTemplate', function (t) {
  t.plan(3);
  var r = new bindings.MyObject();
  t.type(r, 'object');
  t.type(r.instanceProp, 'string');
  t.equal(r.instanceProp, 'an instance property');
});