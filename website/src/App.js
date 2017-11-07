import React, { Component } from 'react';
import { BrowserRouter, Redirect, Route, Switch } from 'react-router-dom'
import './App.css';
import Home from './Home.js';

class App extends Component {
  render() {
    return (
      <BrowserRouter basename="/mexican-train">
        <div className="App">
          <Switch>
            <Route exact path="/" component={Home}/>
            <Redirect to="/"/>
          </Switch>
        </div>
      </BrowserRouter>
    );
  }
}

export default App;
