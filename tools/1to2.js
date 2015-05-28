/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

var fs = require('fs'),
    glob = require('glob'),
    removed = [
      'GetIndexedPropertiesExternalArrayDataLength',
      'GetIndexedPropertiesExternalArrayData',
      'GetIndexedPropertiesExternalArrayDataType',
      'GetIndexedPropertiesPixelData',
      'GetIndexedPropertiesPixelDataLength',
      'HasIndexedPropertiesInExternalArrayData',
      'HasIndexedPropertiesInPixelData',
      'SetIndexedPropertiesToExternalArrayData',
      'SetIndexedPropertiesToPixelData'],
    removedregex = [],
    callbacks = [
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
      'NAN_INDEX_QUERY'],
    callbackregex = [],
    length,
    i;

function replace(file, s) {
  var i, length;

  for (i = 0, length = removedregex.length; i < length; i++) {
    s = s.replace(removedregex[i], function (match) {
    return '// ERROR: Rewrite using Buffer\n' +  match;
    });
  }

  for (i = 0, length = callbackregex.length; i < length; i++) {
    s = s.replace(callbackregex[i], function (match, p1) {
    return p1;
    });
  }

  s = s.replace(/(\W)args(\W)/g, function (match, p1, p2) {
    return [p1, 'info', p2].join('');
  });

  s = s.replace(/NanNew(<(?:v8\:\:)?String>)?\("(.*)"\)/g, function(match, p1, p2) {
    return ['NanNew', p1, '("', p2, '").ToLocalChecked()'].join('');
  });

  s = s.replace(/NanNew(<(?:v8\:\:)?Date>)\(.*\)/g, function(match, p1, p2) {
    return ['NanNew', p1, '(', p2, ').ToLocalChecked()'].join('');
  });

  s = s.replace(/NanNew(<(?:v8\:\:)?RegExp>)\(.*\)/g, function(match, p1, p2) {
    return ['NanNew', p1, '(', p2, ').ToLocalChecked()'].join('');
  });

  s = s.replace(/NanScope\(\)/g, 'NanScope scope');

  s = s.replace(/NanEscapableScope\(\)/g, 'NanEscapableScope scope');

  s = s.replace(/NanEscapeScope/g, 'scope.Escape');

  s = s.replace(/NanReturnValue/g, 'info.GetReturnValue().Set');

  s = s.replace(/NanReturnUndefined\(\)/g, 'return');

  s = s.replace(/NanReturnNull/g, 'info.GetReturnValue().SetNull');

  s = s.replace(/NanAssignPersistent(?:<v8\:\:.*>)?\((.*),\s?(.*)\)/g,
      function(match, p1, p2){
    return [p1, '.Reset(', p2, ')'].join('');
  })

  s = s.replace(/NanDisposePersistent\s*\(\s*(\w+)/g, function(match, p1) {
    return p1 + '.Reset(';
  });

  s = s.replace(/(\W)(?:v8\:\:)?Persistent(\W)/g, function(match, p1, p2) {
    return [p1, 'NanPersistent', p2].join(''); });

  s = s.replace(/NanSetWeak\s*\(\s*(\w+)\s*,/g, function(match, p1) {
    return p1 + '.SetWeak(';
  });

  s = s.replace(/NanObjectWrapHandle\s*\((\w+)/g, function (match, p1) {
    return p1 + '->handle(';
  });

  s = s.replace(/(\W)(?:node\:\:)?ObjectWrap(\W)/g, function(match, p1, p2) {
    return [p1, 'NanObjectWrap', p2].join('');
  });

  s = s.replace(/(\w+)->Set\(/g, function (match, p1) {
    return 'NanSet(' + p1 + ', '
  });

  s = s.replace(/NAN_WEAK_CALLBACK\(([^\)]+)\)/, function (match, p1) {
    return ['template<typename T>\nvoid ',
        p1, '(const NanWeakCallbackInfo<T> &data)'].join('');
  });

  s = s.replace(/NODE_SET_METHOD/g, 'NanSetMethod');

  s = s.replace(/NODE_SET_PROTOTYPE_METHOD/g, 'NanSetPrototypeMethod');

  s = s.replace(/(?:v8\:\:)?TryCatch/g, function() { return 'NanTryCatch'; });

  s = s.replace(/NanAsciiString/g, 'NanUtf8String');

  s = s.replace(/NanUcs2String/g, 'v8::String::Value');

  s = s.replace(/_NAN_/g, 'NAN_');

  fs.writeFile(file, s, function (err) {
    if (err) {
      throw err;
    }
  });
}

function processFile(file) {
  fs.readFile(file, {encoding: 'utf8'}, function (err, data) {
    if (err) {
      throw err;
    }

    replace(file, data);
  });
}

for (i = 0, length = removed.length; i < length; i++) {
  removedregex[i] = new RegExp('^(.*)\\W' + removed[i] + '\\W(.*)', 'm');
}

for (i = 0, length = callbacks.length; i < length; i++) {
  callbackregex[i] =
      new RegExp('(' + callbacks[i] + '\\(.*\\)\\s*\\{)\\s*NanScope\\(\\)\\s*;', 'g');
}

for (i = 2, length = process.argv.length; i < length; i++) {
  glob(process.argv[i], function (err, matches) {
    if (err) {
      throw err;
    }
    matches.forEach(function (currentValue) {
      processFile(currentValue);
    });
  });
}
