const test     = require('tap').test
    , bindings = require('bindings')
	
test('SetPrototypeTemplate', function (t) {
  t.plan(4);
  var r = new bindings('settemplate').MyObject();
  t.type(r, 'object');
  r = Object.getPrototypeOf(r);
  t.type(r, 'object');
  t.type(r.test, 'string');
  t.equal(r.test, 'a prototype property');
});