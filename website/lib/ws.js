'use strict';

var socketio = require('socket.io');

module.exports = function addWebSocketListener(server) {
  listener = socketio(server, {
    path: '/mexican-train/socket.io',
  });

  listener.on('connection', function(socket) {
    console.log('Connection!');
    socket.on('disconnect', function() {
      console.log('Disconnection!');
    });
  });
};

