import React, { Component } from 'react';
import './MainMenu.css';
import { Link } from 'react-router-dom';

class MainMenu extends Component {
  exit() {
    window.history.back();
  }

  render() {
    return (
      <div className="MainMenu">
        <div>
          <h1>Mexican Train</h1>
          <ul>
            <li><Link to="/create-game" replace>Create Game</Link></li>
            <li><Link to="/join-game" replace>Join Game</Link></li>
            <li><Link to="/about" replace>About</Link></li>
            <li><button onClick={this.exit}>Exit</button></li>
          </ul>
        </div>
      </div>
    );
  }
}

export default MainMenu;
