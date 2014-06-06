const test     = require('tap').test
    , bindings = require('bindings')

test('asyncworker', function (t) {
  var worker = bindings('asyncworker').a
    , ticks  = 0
    , called = false
  t.type(worker, 'function')
  function tick () {
    ticks++
    if (!called)
      setTimeout(tick, 0)
  }
  setTimeout(tick, 0)
  worker(200, function () {
    called = true
    t.ok(ticks > 6, 'got plenty of ticks! (' + ticks + ')')
    t.end()
  })
})
