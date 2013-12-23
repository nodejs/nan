const test     = require('tap').test
    , bindings = require('bindings');

test('unsafepersistent', function (t) {
  t.plan(7)

  var persistent = bindings('unsafepersistent')
  t.type(persistent.save1, 'function');
  t.type(persistent.get1, 'function');
  t.type(persistent.dispose1, 'function');

  persistent.save1('a string to save')
  t.equal(persistent.get1(), 'a string to save')
  setTimeout(function () {
    t.equal(persistent.get1(), 'a string to save')
  }, 25)
  setTimeout(function () {
    t.equal(persistent.get1(), 'a string to save')
  }, 50)
  setTimeout(function () {
    persistent.dispose1()
  }, 75)
  setTimeout(function () {
    t.ok(persistent.get1() === undefined, 'no more persistent')
  }, 100)
})
