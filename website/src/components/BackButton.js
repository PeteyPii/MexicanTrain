import React, { Component } from 'react';
import './BackButton.css';
import { Link } from 'react-router-dom';
import FaArrowLeft from 'react-icons/lib/fa/arrow-left';

class BackButton extends Component {
  render() {
    return (
      <Link className="BackButton" to={this.props.to} replace><FaArrowLeft/></Link>
    );
  }
}

export default BackButton;
