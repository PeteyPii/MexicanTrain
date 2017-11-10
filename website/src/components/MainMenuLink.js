import React, { Component } from 'react';
import './MainMenuLink.css';
import { Link } from 'react-router-dom';

class MainMenuLink extends Component {
  render() {
    let className = ['MainMenuLink'];
    if (this.props.disabled) {
      className.push('disabled');
    }
    return (
      <Link className={className.join(' ')} to={this.props.to} replace title={this.props.title}>{this.props.children}</Link>
    );
  }
}

export default MainMenuLink;
