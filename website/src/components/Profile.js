import React, { Component } from 'react';
import './Profile.css';
import BackButton from './BackButton';
import { setUser } from '../actions/actions';
import { connect } from 'react-redux';
import { Redirect } from 'react-router-dom';

class Profile extends Component {
  constructor(props) {
    super(props);
    this.state = {
      name: props.name,
      key: props.key_,
      redirect: false,
    };

    this.handleInputChange = this.handleInputChange.bind(this);
    this.confirm = this.confirm.bind(this);
  }

  componentDidUpdate(prevProps) {
    if (this.props.name !== prevProps.name || this.props.key_ !== prevProps.key_) {
      this.setState({
        name: this.props.name,
        key: this.props.key_,
      });
    }
  }

  handleInputChange(event) {
    const target = event.target;
    let value = target.value;
    const name = target.name;

    value = value.substring(0, 128);
    if (name === 'name') {
      value = value.replace(/[^\d\w-_]/, '');
    }

    this.setState({
      [name]: value
    });
  }

  confirm(event) {
    event.preventDefault();
    this.props.dispatch(setUser(this.state.name, this.state.key));
    this.setState({
      redirect: true
    });
  }

  render() {
    if (this.state.redirect) {
      return (
        <Redirect to="/"/>
      );
    }
    return (
      <div className="Profile">
        <form>
          <input type="text" name="name" spellCheck="false" placeholder="Username" maxLength="128" onChange={this.handleInputChange} value={this.state.name}/>
          <input type="password" name="key" placeholder="Key" maxLength="128" onChange={this.handleInputChange} value={this.state.key}/>
          <button onClick={this.confirm}>Confirm</button>
        </form>
        <BackButton to="/"/>
      </div>
    );
  }
}

function mapStateToProps(state) {
  return {
    name: state.user.name,
    key_: state.user.key, // "key_" because "key" already has a meaning in React
  }
}

export default connect(mapStateToProps)(Profile);
