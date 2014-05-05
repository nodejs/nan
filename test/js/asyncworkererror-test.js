const test     = require('tap').test
    , bindings = require('bindings')

test('asyncworkererror', function (t) {
  var worker = bindings('asyncworkererror').a
  t.type(worker, 'function')
  worker(function (err) {
    t.ok(err)
    t.equal(err.message, 'Error')
    t.end()
  })
})
