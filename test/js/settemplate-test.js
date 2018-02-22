/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

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

test('PropertyAttribute None', function (t) {
  t.plan(3);
  var r = new bindings.MyObject();
  t.type(r, 'object');
  t.type(r.none, 'string');
  t.equal(r.none, 'none');
});

test('PropertyAttribute ReadOnly', function (t) {
  t.plan(4);
  var r = new bindings.MyObject();
  t.type(r, 'object');
  t.type(r.readOnly, 'string');
  t.equal(r.readOnly, 'readOnly');
  r.readOnly = 'changed';
  t.equal(r.readOnly, 'readOnly');
});

test('PropertyAttribute DontEnum', function (t) {
  t.plan(4);
  var r = new bindings.MyObject();
  t.type(r, 'object');
  t.type(r.dontEnum, 'string');
  t.equal(r.dontEnum, 'dontEnum');
  t.notOk(r.propertyIsEnumerable('dontEnum'), 'is not enumerable');
});

test('PropertyAttribute DontDelete', function (t) {
  t.plan(4);
  var r = new bindings.MyObject();
  t.type(r, 'object');
  t.type(r.dontDelete, 'string');
  t.equal(r.dontDelete, 'dontDelete');
  delete r.dontDelete;
  t.equal(r.dontDelete, 'dontDelete');
});
