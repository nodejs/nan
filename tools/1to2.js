/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2015 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

var events = require('events'),
    emitter = new events.EventEmitter(),
    fs = require('fs'),
    glob = require('glob'),
    i, length, removedregex = [];

const removed = [
  'GetIndexedPropertiesExternalArrayDataLength',
  'GetIndexedPropertiesExternalArrayData',
  'GetIndexedPropertiesExternalArrayDataType',
  'GetIndexedPropertiesPixelData',
  'GetIndexedPropertiesPixelDataLength',
  'HasIndexedPropertiesInExternalArrayData',
  'HasIndexedPropertiesInPixelData',
  'SetIndexedPropertiesToExternalArrayData',
  'SetIndexedPropertiesToPixelData'];

length = removed.length;

for (i = 2; i < process.argv.length; i++) {
  glob(process.argv[i], function (err, matches) {
    if (err) {
      throw err;
    }
    matches.forEach(function (currentValue) {
      emitter.emit('file', currentValue);
    });
  });
}

for (i = 0; i < length; i++) {
  removedregex[i] = new RegExp('^(.*)\\W' + removed[i] + '\\W(.*)', 'm');
}

function replace(file, s) {
  var i;

  s = s.replace(/(\W)args(\W)/g, function (match, p1, p2) {
    return [p1, 'info', p2].join('');
  });

  s = s.replace(/NanScope\(\)/g, 'NanScope scope');

  s = s.replace(/NanEscapableScope\(\)/g, 'NanEscapableScope scope');

  s = s.replace(/NanReturnValue/g, 'info.GetReturnValue().Set');

  s = s.replace(/NanReturnUndefined\(\)/g, 'return');

  s = s.replace(/NanReturnNull/g, 'info.GetReturnValue().SetNull');

  s = s.replace(/NanDisposePersistent\s*\(\s*(\w+)/g, function(match, p1) {
    return p1 + '.Reset(';
  });

  s = s.replace(/(?:v8\:\:)?Persistent/g, function() { return 'NanPersistent'; });

  s = s.replace(/NanSetWeak\s*\(\s*(\w+)\s*,/g, function(match, p1) {
    return p1 + '.SetWeak(';
  });

  s = s.replace(/(?:node\:\:)?ObjectWrap/g, function() { return 'NanObjectWrap'; });

  s = s.replace(/NanObjectWrapHandle\s*\((\w+)/g, function (match, p1) {
    return p1 + '.handle(';
  });

  s = s.replace(/NODE_SET_METHOD/g, 'NanSetMethod');

  s = s.replace(/NODE_SET_PROTOTYPE_METHOD/g, 'NanSetPrototypeMethod');

  s = s.replace(/(?:v8\:\:)?TryCatch/g, function() { return 'NanTryCatch'; });

  s = s.replace(/NanAsciiString/g, 'NanUtf8String');

  s = s.replace(/NanUcs2String/g, 'v8::String::Value');

  s = s.replace(/_NAN_/g, 'NAN_');

  for (i = 0; i < length; i++) {
    s = s.replace(removedregex[i], function (match, p1, p2) {
    return '// ERROR: Rewrite using Buffer\n' +  match;
    });
  }

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

emitter.on('file', processFile);
