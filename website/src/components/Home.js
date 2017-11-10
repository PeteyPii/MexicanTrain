import React, { Component } from 'react';
import './Home.css';
import MainMenu from './MainMenu';

class Home extends Component {
  render() {
    return (
      <div className="Home">
        <MainMenu/>
      </div>
    );
  }
}

export default Home;
