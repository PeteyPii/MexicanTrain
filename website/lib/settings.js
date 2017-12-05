var _ = require('lodash');

var settings = {};

try {
  var defaults = require('../defaults.json');
  _.assign(settings, defaults);
} catch (err) {
  // Silently ignore (in case the file is missing)
}

try {
  var userSettings = require('../settings.json');
  _.assign(settings, userSettings);
} catch (err) {
  // Silently ignore (in case the file is missing)
}

function validateSettings(settings) {
  var requiredSettings = [
    'secret_key',
  ];

  for (var i = 0; i < requiredSettings.length; i++) {
    if (typeof settings[requiredSettings[i]] == 'undefined')
      throw new Error('Missing setting \'' + requiredSettings[i] + '\'');
  }
}

try {
  validateSettings(settings);
} catch (err) {
  console.error(err);
  process.kill(process.pid);
}

module.exports = settings;
