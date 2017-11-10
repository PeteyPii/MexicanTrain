export const SET_USER = 'SET_USER';

export function setUser(name, key) {
  return {
    type: SET_USER,
    name: name,
    key: key,
  };
};
