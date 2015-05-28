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
    toconverters = [
      'Boolean',
      'Number',
      'String',
      'Object',
      'Integer',
      'Uint32',
      'Int32'],
    toconvertersregex = [],
    tovalues = [
      ['bool', 'Boolean'],
      ['double', 'Number'],
      ['int64_t', 'Integer'],
      ['uint32_t', 'Uint32'],
      ['int32_t', 'Int32']],
    tovaluesregex = [],
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

  for (i = 0, length = tovaluesregex.length; i < length; i++) {
    s = s.replace(tovaluesregex[i], function (match, p1) {
      return ['NanTo<', tovalues[i][0], '>(',  p1].join('');
    });
  }

  for (i = 0, length = toconverters.length; i < length; i++) {
    s = s.replace(toconvertersregex[i], function (match, p1) {
      return ['NanTo<', toconverters[i], '>(',  p1].join('');
    });
  }

  s = s.replace(/(\S+)->ToDetailString\(/, function (match, p1) {
    return 'NanToDetailString(' + p1;
  });

  s = s.replace(/(\S+)->ToArrayIndex\(/, function (match, p1) {
    return 'NanToArrayIndex(' + p1;
  });


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

  s = s.replace(/(?:v8\:\:)?TryCatch/g, function() { return 'NanTryCatch'; });

  s = s.replace(/(\w+)->StackTrace\(\)/g, function (match) {
    return match + '.ToLocalChecked()';
  });

  s = s.replace(/(\w+)->Equals\((\w+)/, function (match, p1, p2) {
    return ['NanEquals(', p1, ', ', p2].join('');
  });

  s = s.replace(/(\w+)->NewInstance\(/g, function (match, p1) {
    return 'NanNewInstance(' + p1;
  });

  s = s.replace(/(\w+)->GetFunction\(/g, function (match, p1) {
    return 'NanGetFunction(' + p1;
  });

  s = s.replace(/(\w+)->Set\(/g, function (match, p1) {
    return 'NanSet(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->ForceSet\(/g, function (match, p1) {
    return 'NanForceSet(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->Get\(/g, function (match, p1) {
    return 'NanGet(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->GetPropertyAttributes\(/g, function (match, p1) {
    return 'NanGetPropertyAttributes(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->Has\(/g, function (match, p1) {
    return 'NanHas(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->Delete\(/g, function (match, p1) {
    return 'NanDelete(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->GetPropertyNames\(/g, function (match, p1) {
    return 'NanGetPropertyNames(' + p1;
  });

  s = s.replace(/(\w+)->GetOwnPropertyNames\(/g, function (match, p1) {
    return 'NanGetOwnPropertyNames(' + p1;
  });

  s = s.replace(/(\w+)->SetPrototype\(/g, function (match, p1) {
    return 'NanSetPrototype(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->ObjectProtoToString\(/g, function (match, p1) {
    return 'NanObjectProtoToString(' + p1;
  });

  s = s.replace(/(\w+)->HasOwnProperty\(/g, function (match, p1) {
    return 'NanHasOwnProperty(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->HasRealNamedProperty\(/g, function (match, p1) {
    return 'NanHasRealNamedProperty(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->HasRealIndexedProperty\(/g, function (match, p1) {
    return 'NanHasRealIndexedProperty(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->HasRealNamedCallbackProperty\(/g, function (match, p1) {
    return 'NanHasRealNamedCallbackProperty(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->GetRealNamedPropertyInPrototypeChain\(/g,
      function (match, p1) {
    return 'NanGetRealNamedPropertyInPrototypeChain(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->GetRealNamedProperty\(/g, function (match, p1) {
    return 'NanGetRealNamedProperty(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->CallAsFunction\(/g,
      function (match, p1) {
    return 'NanCallAsFunction(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->CallAsConstructor\(/g,
      function (match, p1) {
    return 'NanCallAsConstructor(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->GetSourceLine\(/g,
      function (match, p1) {
    return 'NanGetSourceLine(' + p1;
  });

  s = s.replace(/(\w+)->GetLineNumber\(/g,
      function (match, p1) {
    return 'NanGetLineNumber(' + p1;
  });

  s = s.replace(/(\w+)->GetStartColumn\(/g,
      function (match, p1) {
    return 'NanGetStartColumn(' + p1;
  });

  s = s.replace(/(\w+)->GetEndColumn\(/g,
      function (match, p1) {
    return 'NanGetEndColumn(' + p1;
  });

  s = s.replace(/(\w+)->CloneElementAt\(/g,
      function (match, p1) {
    return 'NanCloneElementAt(' + p1 + ', ';
  });

  s = s.replace(/(\w+)->\(/g,
      function (match, p1) {
    return 'NanGetStartColumn(' + p1;
  });

  s = s.replace(/NAN_WEAK_CALLBACK\(([^\)]+)\)/, function (match, p1) {
    return ['template<typename T>\nvoid ',
        p1, '(const NanWeakCallbackInfo<T> &data)'].join('');
  });

  s = s.replace(/NODE_SET_METHOD/g, 'NanSetMethod');

  s = s.replace(/NODE_SET_PROTOTYPE_METHOD/g, 'NanSetPrototypeMethod');

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
  callbackregex[i] = new RegExp(
      '(' + callbacks[i] + '\\(.*\\)\\s*\\{)\\s*NanScope\\(\\)\\s*;', 'g');
}

for (i = 0, length = toconverters.length; i < length; i++) {
  toconvertersregex[i] =
      new RegExp('(\\S+)->To' + toconverters[i] + '\\(', 'g');
}

for (i = 0, length = tovalues.length; i < length; i++) {
  tovaluesregex[i] =
      new RegExp('(\\S+)->' + tovalues[i][1] + 'Value\\(', 'g');
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
