import React, { Component } from 'react';
import './MainMenu.css';
import MainMenuLink from './MainMenuLink';
import { connect } from 'react-redux';

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
            {this.props.userSelected &&
              <div>
                <li><MainMenuLink to="/create-game" replace>Create Game</MainMenuLink></li>
                <li><MainMenuLink to="/join-game" replace>Join Game</MainMenuLink></li>
              </div>
            }
            {!this.props.userSelected &&
              <div>
                <li><MainMenuLink to="/create-game" replace disabled title="Set up your profile before playing.">Create Game</MainMenuLink></li>
                <li><MainMenuLink to="/join-game" replace disabled title="Set up your profile before playing.">Join Game</MainMenuLink></li>
              </div>
            }
            <li><MainMenuLink to="/profile" replace>Profile</MainMenuLink></li>
            <li><MainMenuLink to="/about" replace>About</MainMenuLink></li>
            <li><button onClick={this.exit}>Exit</button></li>
          </ul>
        </div>
      </div>
    );
  }
}

function mapStateToProps(state) {
  const { user } = state;
  if (user.name) {
    return {
      userSelected: true,
    };
  } else {
    return {
      userSelected: false,
    };
  }
}

export default connect(mapStateToProps)(MainMenu);
