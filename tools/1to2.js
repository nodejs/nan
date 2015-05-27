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

  s = s.replace(/(\W)args(\W)/, function (match, p1, p2) {
    return [p1, 'info', p2].join('');
  });

  s = s.replace('NanScope()', 'NanScope scope');

  s = s.replace('NanEscapableScope()', 'NanEscapableScope scope');

  s = s.replace('NanReturnValue', 'info.GetReturnValue().Set');

  s = s.replace('NanReturnUndefined()', 'return');

  s = s.replace('NanReturnNull', 'info.GetReturnValue().SetNull');

  s = s.replace(/(?:v8\:\:)?Persistent/, function() { return 'NanPersistent'; });

  s = s.replace(/NanSetWeak\s*\(\s*(\w+)\s*,/, function(match, p1) {
    return p1 + '.SetWeak(';
  });

  s = s.replace(/(?:node\:\:)?ObjectWrap/, function() { return 'NanObjectWrap'; });

  s = s.replace(/NanObjectWrapHandle\s*\((\w+)/, function (match, p1) {
    return p1 + '.handle(';
  });

  s = s.replace(/(?:v8\:\:)?TryCatch/, function() { return 'NanTryCatch'; });

  s = s.replace('NanAsciiString', 'NanUtf8String');

  s = s.replace('NanUcs2String', 'v8::String::Value');

  s = s.replace(/NanDisposePersistent\s*\(\s*(\w+)/, function(match, p1) {
    return p1 + '.Dispose(';
  });

  s = s.replace('_NAN_', 'NAN_');

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
