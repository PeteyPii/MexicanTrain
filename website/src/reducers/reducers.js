import { combineReducers } from 'redux';

import { SET_USER } from '../actions/actions';

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

const rootReducer = combineReducers({
  user: user,
});

export default rootReducer;
