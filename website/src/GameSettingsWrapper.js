import { GameSettings } from '../gen/GameSettings_pb';

export const MIN_NUMBER_OF_PLAYERS = 1;
export const MAX_NUMBER_OF_PLAYERS = 8;
export const MIN_MAX_PIPS = 0;
export const MAX_MAX_PIPS = 18;
export const MIN_STARTING_HAND_SIZE = 0;
export const MAX_STARTING_HAND_SIZE = 30;

export default class GameSettingsWrapper {
  constructor() {
    this._settings = new GameSettings();
    this.setNumberOfPlayers(1);
    this.setMaxPips(0);
    this.setStartingHandSize(0);
  }

  getNumberOfPlayers() {
    return this._settings.getNumberOfPlayers();
  }

  setNumberOfPlayers(val) {
    this._settings.setNumberOfPlayers(val);
  }

  getMaxPips() {
    return this._settings.getMaxPips();
  }

  setMaxPips(val) {
    this._settings.setMaxPips(val);
  }

  getStartingHandSize() {
    return this._settings.getStartingHandSize();
  }

  setStartingHandSize(val) {
    this._settings.setStartingHandSize(val);
  }

  isValid() {
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
}
