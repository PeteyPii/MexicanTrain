'use strict';

var GameSettings = require('../gen/GameSettings_pb').GameSettings;

var MIN_NUMBER_OF_PLAYERS = 1;
var MAX_NUMBER_OF_PLAYERS = 8;
var MIN_MAX_PIPS = 0;
var MAX_MAX_PIPS = 18;
var MIN_STARTING_HAND_SIZE = 0;
var MAX_STARTING_HAND_SIZE = 30;

function GameSettingsWrapper() {
  this._settings = new GameSettings();
  this.setNumberOfPlayers(1);
  this.setMaxPips(0);
  this.setStartingHandSize(0);
}

GameSettingsWrapper.prototype.getNumberOfPlayers = function getNumberOfPlayers() {
  return this._settings.getNumberOfPlayers();
}

GameSettingsWrapper.prototype.setNumberOfPlayers = function setNumberOfPlayers(val) {
  this._settings.setNumberOfPlayers(val);
}

GameSettingsWrapper.prototype.getMaxPips = function getMaxPips() {
  return this._settings.getMaxPips();
}

GameSettingsWrapper.prototype.setMaxPips = function setMaxPips(val) {
  this._settings.setMaxPips(val);
}

GameSettingsWrapper.prototype.getStartingHandSize = function getStartingHandSize() {
  return this._settings.getStartingHandSize();
}

GameSettingsWrapper.prototype.setStartingHandSize = function setStartingHandSize(val) {
  this._settings.setStartingHandSize(val);
}

GameSettingsWrapper.prototype.getUnderlyingSettings = function getUnderlyingSettings() {
  return this._settings;
}

GameSettingsWrapper.prototype.isValid = function isValid() {
  if (this.getNumberOfPlayers() < MIN_NUMBER_OF_PLAYERS || this.getNumberOfPlayers() > MAX_NUMBER_OF_PLAYERS) {
    return false;
  }
  if (this.getMaxPips() < MIN_MAX_PIPS || this.getMaxPips() > MAX_MAX_PIPS) {
    return false;
  }
  if (this.getStartingHandSize() < MIN_STARTING_HAND_SIZE || this.getStartingHandSize() > MAX_STARTING_HAND_SIZE) {
    return false;
  }
  return true;
}

module.exports = {
  GameSettingsWrapper: GameSettingsWrapper,
  MIN_NUMBER_OF_PLAYERS: MIN_NUMBER_OF_PLAYERS,
  MAX_NUMBER_OF_PLAYERS: MAX_NUMBER_OF_PLAYERS,
  MIN_MAX_PIPS: MIN_MAX_PIPS,
  MAX_MAX_PIPS: MAX_MAX_PIPS,
  MIN_STARTING_HAND_SIZE: MIN_STARTING_HAND_SIZE,
  MAX_STARTING_HAND_SIZE: MAX_STARTING_HAND_SIZE,
};
