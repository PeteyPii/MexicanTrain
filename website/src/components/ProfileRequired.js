import React, { Component } from 'react';
import { connect } from 'react-redux';
import { Redirect } from 'react-router-dom';

class ProfileRequired extends Component {
  render() {
    if (!this.props.name || !this.props.key_) {
      return (
        <Redirect to="/profile"/>
      );
    }

    return null;
  }
}

function mapStateToProps(state) {
  return {
    name: state.user.name,
    key_: state.user.key, // "key_" because "key" already has a meaning in React
  }
}

export default connect(mapStateToProps)(ProfileRequired);
