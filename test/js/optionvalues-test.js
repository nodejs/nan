/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'optionvalues' })
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

  t.type(bindings.o, 'function');
  actobj = bindings.o(obj);
  t.deepEqual(actobj, expobj);
});
