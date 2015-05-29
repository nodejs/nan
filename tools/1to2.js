#!/usr/bin/env node
/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

var commander = require('commander'),
    fs = require('fs'),
    glob = require('glob'),
    groups = [],
    length,
    i;

fs.readFile('package.json', 'utf8', function (err, data) {
  if (err) {
    throw err;
  }

  commander
      .version(JSON.parse(data).version)
      .usage('[options] <file ...>')
      .parse(process.argv);

  if (!process.argv.slice(2).length) {
    commander.outputHelp();
  }
});

groups.push([0, [
  '_NAN_',
  'NODE_SET_METHOD',
  'NODE_SET_PROTOTYPE_METHOD',
  'NanAsciiString',
  'NanEscapeScope',
  'NanReturnValue',
  'NanUcs2String'].join('|')]);

groups.push([1, ['(', [
  'NanEscapableScope',
  'NanReturnNull',
  'NanReturnUndefined',
  'NanScope'].join('|'), ')\\(\\)'].join('')]);

groups.push([2, '(?:(?:v8\\:\\:)?|(Nan)?)(TryCatch)']);

groups.push([1, ['(NanNew)', '(\\("[^\\"]*"[^\\)]*\\))(?!\\.ToLocalChecked\\(\\))'].join('')]);

groups.push([1, ['^.*?(', [
      'GetIndexedPropertiesExternalArrayDataLength',
      'GetIndexedPropertiesExternalArrayData',
      'GetIndexedPropertiesExternalArrayDataType',
      'GetIndexedPropertiesPixelData',
      'GetIndexedPropertiesPixelDataLength',
      'HasIndexedPropertiesInExternalArrayData',
      'HasIndexedPropertiesInPixelData',
      'SetIndexedPropertiesToExternalArrayData',
      'SetIndexedPropertiesToPixelData'].join('|'), ')'].join('')]);

groups.push([2, ['((', [
      'NAN_METHOD',
      'NAN_GETTER',
      'NAN_SETTER',
      'NAN_PROPERTY_GETTER',
      'NAN_PROPERTY_SETTER',
      'NAN_PROPERTY_ENUMERATOR',
      'NAN_PROPERTY_DELETER',
      'NAN_PROPERTY_QUERY',
      'NAN_INDEX_GETTER',
      'NAN_INDEX_SETTER',
      'NAN_INDEX_ENUMERATOR',
      'NAN_INDEX_DELETER',
      'NAN_INDEX_QUERY'].join('|'), ')\\([^\\)]*\\)\\s*\\{)\\s*NanScope\\(\\)\\s*;'].join('')]);

groups.push([3, ['([\\s\\(\\)])([^\\s\\(\\)]+)->(', [
      'Boolean',
      'Number',
      'String',
      'Object',
      'Integer',
      'Uint32',
      'Int32'].join('|'), ')\\('].join('')]);

groups.push([3, ['([\\s\\(\\)])([^\\s\\(\\)]+)->((?:', [
      'Boolean',
      'Number',
      'Integer',
      'Uint32',
      'Int32'].join('|'), ')Value)\\('].join('')]);

groups.push([1, '(NAN_WEAK_CALLBACK)\\(([^\\s\\)]+)\\)']);

groups.push([1, ['(', [
  'NanDisposePersistent',
  'NanObjectWrapHandle'].join('|'), ')\\s*\\(\\s*([^\\s\\)]+)'].join('')]);

groups.push([1, '(NanMakeWeakPersistent)\\s*\\(\\s*([^\\s,]+)\\s*,\\s*']);

groups.push([3, ['([\\s])([^\\s]+)->(', [
  'GetEndColumn',
  'GetFunction',
  'GetLineNumber',
  'NewInstance',
  'GetPropertyNames',
  'GetOwnPropertyNames',
  'GetSourceLine',
  'GetStartColumn',
  'ObjectProtoToString',
  'ToArrayIndex',
  'ToDetailString',
  'CallAsConstructor',
  'CallAsFunction',
  'CloneElementAt',
  'Delete',
  'ForceSet',
  'Get',
  'GetPropertyAttributes',
  'GetRealNamedProperty',
  'GetRealNamedPropertyInPrototypeChain',
  'Has',
  'HasOwnProperty',
  'HasRealIndexedProperty',
  'HasRealNamedCallbackProperty',
  'HasRealNamedProperty',
  'Set',
  'SetAccessor',
  'SetIndexedPropertyHandler',
  'SetNamedPropertyHandler',
  'SetPrototype'].join('|'), ')\\('].join('')]);

groups.push([2, ['NanNew(<(?:v8\\:\\:)?(', ['Date', 'String', 'RegExp'].join('|'), ')>)(\\([^\\)]*\\))(?!\\.ToLocalChecked\\(\\))'].join('')]);

groups.push([3, '([\\s\\(\\)])([^\\s\\(\\)]+)->(Equals)\\(([^\\s\\)]+)']);

groups.push([1, '(NanAssignPersistent)(?:<v8\\:\\:[^>]+>)?\\(([^,]+),\\s*']);

groups.push([2, '(\\W)(args)(\\W)'])

groups.push([2, '(\\W)(?:node\\:\\:)?(ObjectWrap)(\\W)']);

groups.push([2, '(\\W)(?:v8\\:\\:)?(Persistent)(\\W)']);

function groupcount(s) {
  var positive = s.match(/\((?!\?)/g),
      negative = s.match(/\\\(/g);
  return (positive ? positive.length : 0) - (negative ? negative.length : 0);
}

var total = 0;
for (i = 1, length = groups.length; i < length; i++) {
	total += groupcount(groups[i - 1][1]);
	groups[i][0] += total;
}

master = new RegExp(groups.map(function (a) { return a[1]; }).join('|'), 'gm');

function replace() {
      switch (arguments[groups[0][0]]) {
        case '_NAN_':
          return 'NAN_';
        case 'NODE_SET_METHOD':
          return 'NanSetMethod';
        case 'NODE_SET_PROTOTYPE_METHOD':
          return 'NanSetPrototypeMethod';
        case 'NanAsciiString':
          return 'NanUtf8String';
        case 'NanEscapeScope':
          return 'scope.Escape';
        case 'NanReturnNull':
          return 'info.GetReturnValue().SetNull';
        case 'NanReturnValue':
          return 'info.GetReturnValue().Set';
        case 'NanUcs2String':
          return 'v8::String::Value';
        default:
      }

      switch (arguments[groups[1][0]]) {
        case 'NanEscapableScope':
          return 'NanEscapableScope scope'
        case 'NanReturnUndefined':
          return 'return';
        case 'NanScope':
          return 'NanScope scope';
        default:
      }

      if (arguments[groups[2][0]] === 'TryCatch') {
        return arguments[groups[2][0] - 1] ? arguments[0] : 'NanTryCatch';
      }

      if (arguments[groups[3][0]] === 'NanNew') {
        return [arguments[0], '.ToLocalChecked()'].join('');
      }

      switch (arguments[groups[4][0]]) {
        case 'GetIndexedPropertiesExternalArrayData':
        case 'GetIndexedPropertiesExternalArrayDataLength':
        case 'GetIndexedPropertiesExternalArrayDataType':
        case 'GetIndexedPropertiesPixelData':
        case 'GetIndexedPropertiesPixelDataLength':
        case 'HasIndexedPropertiesInExternalArrayData':
        case 'HasIndexedPropertiesInPixelData':
        case 'SetIndexedPropertiesToExternalArrayData':
        case 'SetIndexedPropertiesToPixelData':
          return '// ERROR: Rewrite using Buffer\n' +  arguments[0];
        default:
      }

      switch (arguments[groups[5][0]]) {
        case 'NAN_GETTER':
        case 'NAN_METHOD':
        case 'NAN_SETTER':
        case 'NAN_INDEX_DELETER':
        case 'NAN_INDEX_ENUMERATOR':
        case 'NAN_INDEX_GETTER':
        case 'NAN_INDEX_QUERY':
        case 'NAN_INDEX_SETTER':
        case 'NAN_PROPERTY_DELETER':
        case 'NAN_PROPERTY_ENUMERATOR':
        case 'NAN_PROPERTY_GETTER':
        case 'NAN_PROPERTY_QUERY':
        case 'NAN_PROPERTY_SETTER':
          return arguments[groups[5][0] - 1];
        default:
      }

      switch (arguments[groups[6][0]]) {
        case 'Boolean':
        case 'Int32':
        case 'Integer':
        case 'Number':
        case 'Object':
        case 'String':
        case 'Uint32':
          return [arguments[groups[6][0] - 2], 'NanTo<v8::', arguments[groups[6][0]], '>(',  arguments[groups[6][0] - 1]].join('');
        default:
      }

      switch (arguments[groups[7][0]]) {
        case 'BooleanValue':
          return [arguments[groups[7][0] - 2], 'NanTo<bool>(', arguments[groups[7][0] - 1]].join('');
        case 'Int32Value':
          return [arguments[groups[7][0] - 2], 'NanTo<int32_t>(', arguments[groups[7][0] - 1]].join('');
        case 'IntegerValue':
          return [arguments[groups[7][0] - 2], 'NanTo<int64_t>(', arguments[groups[7][0] - 1]].join('');
        case 'Uint32Value':
          return [arguments[groups[7][0] - 2], 'NanTo<uint32_t>(', arguments[groups[7][0] - 1]].join('');
        default:
      }

      if (arguments[groups[8][0]] === 'NAN_WEAK_CALLBACK') {
        return ['template<typename T>\nvoid ',
          arguments[groups[8][0] + 1], '(const NanWeakCallbackInfo<T> &data)'].join('');
      }

      switch (arguments[groups[9][0]]) {
        case 'NanDisposePersistent':
          return [arguments[groups[9][0] + 1], '.Reset('].join('');
        case 'NanObjectWrapHandle':
          return [arguments[groups[9][0] + 1], '->handle('].join('');
        default:
      }

      if (arguments[groups[10][0]] === 'NanMakeWeakPersistent') {
        return arguments[groups[10][0] + 1] + '.SetWeak(';
      }

      switch (arguments[groups[11][0]]) {
        case 'GetEndColumn':
        case 'GetFunction':
        case 'GetLineNumber':
        case 'GetOwnPropertyNames':
        case 'GetPropertyNames':
        case 'GetSourceLine':
        case 'GetStartColumn':
        case 'NewInstance':
        case 'ObjectProtoToString':
        case 'ToArrayIndex':
        case 'ToDetailString':
          return [arguments[groups[11][0] - 2], 'Nan', arguments[groups[11][0]], '(', arguments[groups[11][0] - 1]].join('');
        case 'CallAsConstructor':
        case 'CallAsFunction':
        case 'CloneElementAt':
        case 'Delete':
        case 'ForceSet':
        case 'Get':
        case 'GetPropertyAttributes':
        case 'GetRealNamedProperty':
        case 'GetRealNamedPropertyInPrototypeChain':
        case 'Has':
        case 'HasOwnProperty':
        case 'HasRealIndexedProperty':
        case 'HasRealNamedCallbackProperty':
        case 'HasRealNamedProperty':
        case 'Set':
        case 'SetAccessor':
        case 'SetIndexedPropertyHandler':
        case 'SetNamedPropertyHandler':
        case 'SetPrototype':
          return [arguments[groups[11][0] - 2], 'Nan', arguments[groups[11][0]], '(', arguments[groups[11][0] - 1], ', '].join('');
        default:
      }

      switch (arguments[groups[12][0]]) {
        case 'Date':
        case 'String':
        case 'RegExp':
          return ['NanNew', arguments[groups[12][0] - 1], arguments[groups[12][0] + 1], '.ToLocalChecked()'].join('');
        default:
      }

      if (arguments[groups[13][0]] === 'Equals') {
        return [arguments[groups[13][0] - 1], 'NanEquals(', arguments[groups[13][0] - 1], ', ', arguments[groups[13][0] + 1]].join('');
      }

      if (arguments[groups[14][0]] === 'NanAssignPersistent') {
        return [arguments[groups[14][0] + 1], '.Reset('].join('');
      }

      if (arguments[groups[15][0]] === 'args') {
        return [arguments[groups[15][0] - 1], 'info', arguments[groups[15][0] + 1]].join('');
      }

      if (arguments[groups[16][0]] === 'ObjectWrap') {
        return [arguments[groups[16][0] - 1], 'NanObjectWrap', arguments[groups[16][0] + 1]].join('');
      }

      if (arguments[groups[17][0]] === 'Persistent') {
        return [arguments[groups[17][0] - 1], 'NanPersistent', arguments[groups[17][0] + 1]].join('');
      }

      throw 'Unhandled match: ' + arguments[0];
}
function processFile(file) {
  fs.readFile(file, {encoding: 'utf8'}, function (err, data) {
    if (err) {
      throw err;
    }

    fs.writeFile(file, data.replace(master, replace).replace(master, replace), function (err) {
      if (err) {
        throw err;
      }
    });
  });
}

for (i = 2, length = process.argv.length; i < length; i++) {
  glob(process.argv[i], function (err, matches) {
    if (err) {
      throw err;
    }
    matches.forEach(processFile);
  });
}
