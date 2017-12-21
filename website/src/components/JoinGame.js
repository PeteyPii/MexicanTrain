import React, { Component } from 'react';
import './JoinGame.css';
import BackButton from './BackButton';
import { setDefaultGameSettings } from '../actions/actions';
import { connect } from 'react-redux';
import { GameSettingsWrapper } from '../../common/GameSettingsWrapper';
import ProfileRequired from './ProfileRequired';
import PropTypes from 'prop-types';
import socket from '../clientSocket';

import FaSpinner from 'react-icons/lib/fa/spinner';


class JoinGame extends Component {
  constructor(props) {
    super(props);
    this.state = {
    };
  }

  render() {
    return (
      <div className="JoinGame">
        <ProfileRequired/>
        <div className="window">
        kek
          <div className="list">
          </div>
        </div>
        <span className="loading"><FaSpinner/></span>
        <BackButton to="/"/>
      </div>
    );
  }
}

JoinGame.contextTypes = {
  store: PropTypes.object,
};

export default JoinGame;
