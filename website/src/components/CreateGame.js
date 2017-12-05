import React, { Component } from 'react';
import './CreateGame.css';
import BackButton from './BackButton';
import { setDefaultGameSettings } from '../actions/actions';
import { connect } from 'react-redux';
import {
  GameSettingsWrapper,
  MIN_NUMBER_OF_PLAYERS,
  MAX_NUMBER_OF_PLAYERS,
  MIN_MAX_PIPS,
  MAX_MAX_PIPS,
  MIN_STARTING_HAND_SIZE,
  MAX_STARTING_HAND_SIZE,
} from '../../common/GameSettingsWrapper';
import ProfileRequired from './ProfileRequired';
import PropTypes from 'prop-types';
import socket from '../clientSocket';


class CreateGame extends Component {
  constructor(props) {
    super(props);
    this.state = {
      numberOfPlayers: this.props.numberOfPlayers,
      maxPips: this.props.maxPips,
      startingHandSize: this.props.startingHandSize,
    };

    this.handleInputChange = this.handleInputChange.bind(this);
    this.create = this.create.bind(this);
  }

  componentDidUpdate(prevProps) {
    if (this.props.numberOfPlayers !== prevProps.numberOfPlayers || this.props.maxPips !== prevProps.maxPips || this.props.startingHandSize !== prevProps.startingHandSize) {
      this.setState({
        numberOfPlayers: this.props.numberOfPlayers,
        maxPips: this.props.maxPips,
        startingHandSize: this.props.startingHandSize,
      });
    }
  }

  handleInputChange(event) {
    const target = event.target;
    const value = target.value;
    const name = target.name;

    if (value === '') {
      this.setState({
        [name]: value
      });
      return;
    }

    const settings = new GameSettingsWrapper();
    settings.setNumberOfPlayers(this.state.numberOfPlayers);
    settings.setMaxPips(this.state.maxPips);
    settings.setStartingHandSize(this.state.startingHandSize);
    switch (name) {
      case 'numberOfPlayers':
        settings.setNumberOfPlayers(value);
        break;
      case 'maxPips':
        settings.setMaxPips(value);
        break;
      case 'startingHandSize':
        settings.setStartingHandSize(value);
        break;
      default:
        throw Error('Input case not handled.');
    }

    if (settings.isValid()) {
      this.setState({
        numberOfPlayers: settings.getNumberOfPlayers(),
        maxPips: settings.getMaxPips(),
        startingHandSize: settings.getStartingHandSize(),
      });
    }
  }

  create(event) {
    event.preventDefault();
    if (this.state.numberOfPlayers === '' || this.state.maxPips === '' || this.state.startingHandSize === '') {
      return;
    }

    const settings = new GameSettingsWrapper();
    settings.setNumberOfPlayers(this.state.numberOfPlayers);
    settings.setMaxPips(this.state.maxPips);
    settings.setStartingHandSize(this.state.startingHandSize);
    this.props.dispatch(setDefaultGameSettings(settings));

    const state = this.context.store.getState();
    socket.emit('create_game', {
      user: state.user,
      gameSettings: state.defaultGameSettings,
    });
  }

  render() {
    return (
      <div className="CreateGame">
        <ProfileRequired/>
        <form>
          <label> Number of Players ({MIN_NUMBER_OF_PLAYERS} - {MAX_NUMBER_OF_PLAYERS})
            <input type="number" name="numberOfPlayers" onChange={this.handleInputChange} value={this.state.numberOfPlayers}/>
          </label>
          <label> Max Pips ({MIN_MAX_PIPS} - {MAX_MAX_PIPS})
            <input type="number" name="maxPips" onChange={this.handleInputChange} value={this.state.maxPips}/>
          </label>
          <label> Starting Hand Size ({MIN_STARTING_HAND_SIZE} - {MAX_STARTING_HAND_SIZE})
            <input type="number" name="startingHandSize" onChange={this.handleInputChange} value={this.state.startingHandSize}/>
          </label>
          <button onClick={this.create}>Create Game</button>
        </form>
        <BackButton to="/"/>
      </div>
    );
  }
}

function mapStateToProps(state) {
  return {
    numberOfPlayers: state.defaultGameSettings.numberOfPlayers,
    maxPips: state.defaultGameSettings.maxPips,
    startingHandSize: state.defaultGameSettings.startingHandSize,
  };
}

CreateGame.contextTypes = {
  store: PropTypes.object,
};

export default connect(mapStateToProps)(CreateGame);
