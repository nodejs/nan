const test     = require('tap').test
    , bindings = require('bindings');

// same as fromv8string test but split over multiple compile units
// just to test that there aren't any conflicts from including nan.h
// more than once in project files
test('multifile', function (t) {
  t.plan(2);
  t.type(bindings('multifile').r, 'function');
  t.equal(bindings('multifile').r('a string value'), 'a string value');
});
