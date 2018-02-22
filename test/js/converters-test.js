/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

const test     = require('tap').test
    , testRoot = require('path').resolve(__dirname, '..')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'converters' });

test('converters', function (t) {
  t.plan(32);

  var converters = bindings;
  t.type(converters.toBoolean, 'function');
  t.type(converters.toNumber, 'function');
  t.type(converters.toString, 'function');
  t.type(converters.toDetailString, 'function');
  t.type(converters.toFunction, 'function');
  t.type(converters.toObject, 'function');
  t.type(converters.toInteger, 'function');
  t.type(converters.toUint32, 'function');
  t.type(converters.toInt32, 'function');
  t.type(converters.toArrayIndex, 'function');
  t.type(converters.booleanValue, 'function');
  t.type(converters.numberValue, 'function');
  t.type(converters.integerValue, 'function');
  t.type(converters.uint32Value, 'function');
  t.type(converters.int32Value, 'function');
  t.equal(converters.toBoolean(true), true);
  t.equal(converters.toNumber(15.3), 15.3);
  t.equal(converters.toString('sol'), 'sol');
  t.equal(converters.toDetailString('sol'), 'sol');
  t.equal(converters.toFunction(test), test);
  t.strictDeepEqual(converters.toObject({prop : 'x'}), {prop : 'x'});
  t.equal(converters.toInteger(12), 12);
  t.equal(converters.toUint32(12), 12);
  t.equal(converters.toInt32(-12), -12);
  t.equal(converters.toArrayIndex('12'), 12);
  t.equal(converters.booleanValue(true), true);
  t.equal(converters.numberValue(15.3), 15.3);
  t.equal(converters.integerValue(12), 12);
  t.equal(converters.uint32Value(12), 12);
  t.equal(converters.int32Value(-12), -12);

  var conversionFailed = {};
  t.equal(converters.toFunction(null, conversionFailed), conversionFailed);
  t.equal(converters.toFunction({}, conversionFailed), conversionFailed);
});
