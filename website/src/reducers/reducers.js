import { combineReducers } from 'redux';

import {
  SET_USER,
  SET_DEFAULT_GAME_SETTINGS,
} from '../actions/actions';

function user(state = { name: '', key: '' }, action) {
  switch (action.type) {
    case SET_USER:
      return {
        name: action.name,
        key: action.key,
      };
    default:
      return state;
  }
}

function defaultGameSettings(state = { numberOfPlayers: 4, maxPips: 12, startingHandSize: 15 }, action) {
  switch (action.type) {
    case SET_DEFAULT_GAME_SETTINGS:
      return {
        numberOfPlayers: action.gameSettings.getNumberOfPlayers(),
        maxPips: action.gameSettings.getMaxPips(),
        startingHandSize: action.gameSettings.getStartingHandSize(),
      };
    default:
      return state;
  }
}

const rootReducer = combineReducers({
  user: user,
  defaultGameSettings: defaultGameSettings,
});

export default rootReducer;
