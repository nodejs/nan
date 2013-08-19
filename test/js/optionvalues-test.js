const test     = require('tap').test
    , bindings = require('bindings')
    , xtend    = require('xtend');

test('optionvalues', function (t) {
  t.plan(2);
  var obj = {
          'true'  : true
        , 'false' : false
        , i       : 100
        , f       : 200.2
      }
    , expobj = xtend(obj, {
          dt : true  // default true
        , df : false // default false
        , f  : 200   // truncate
        , di : 111
      })
    , actobj;

  t.type(bindings('optionvalues').o, 'function');
  actobj = bindings('optionvalues').o(obj);
  t.deepEqual(actobj, expobj);
});
