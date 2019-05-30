if (global.gc) {
  module.exports = global.gc;
  return;
}

var v8 = require('v8');
var vm = require('vm');

v8.setFlagsFromString('--expose_gc');
module.exports = vm.runInNewContext('gc');
