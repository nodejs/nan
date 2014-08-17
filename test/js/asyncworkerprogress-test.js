const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'asyncworkerprogress' });

test('asyncworkerprogress', function (t) {
  var worker = bindings.a
    , progressed = 0
  worker(100, 5, function(i) {
    t.ok(i === progressed, 'got the progress updates #' + i);
    progressed++;
  }, function () {
    t.ok(progressed === 5, 'got all progress updates')
    t.end()
  })
})
