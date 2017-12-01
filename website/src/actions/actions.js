export const SET_USER = 'SET_USER';
export const SET_DEFAULT_GAME_SETTINGS = 'SET_DEFAULT_GAME_SETTINGS';

export function setUser(name, key) {
  return {
    type: SET_USER,
    name: name,
    key: key,
  };
};

export function setDefaultGameSettings(gameSettings) {
  return {
    type: SET_DEFAULT_GAME_SETTINGS,
    gameSettings: gameSettings,
  };
};
