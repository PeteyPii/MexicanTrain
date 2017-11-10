import React, { Component } from 'react';
import { BrowserRouter, Redirect, Route, Switch } from 'react-router-dom';
import { Provider } from 'react-redux';
import './Root.css';
import Home from './Home';
import Profile from './Profile';
import configureStore from '../configureStore';

const store = configureStore();

class Root extends Component {
  render() {
    return (
      <Provider store={store}>
        <BrowserRouter basename="/mexican-train">
          <div className="Root">
            <Switch>
              <Route exact path="/" component={Home}/>
              <Route exact path="/profile" component={Profile}/>
              <Redirect to="/"/>
            </Switch>
          </div>
        </BrowserRouter>
      </Provider>
    );
  }
}

export default Root;
