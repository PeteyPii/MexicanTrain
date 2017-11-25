'use strict';

var net = require('net');

var socketio = require('socket.io');
var nbo = require('network-byte-order');

var RequestType = require('../gen/RequestType_pb').RequestType;

function Backend(frontendServer) {
  this.listener = socketio(frontendServer, {
    path: '/mexican-train/socket.io',
  });

  this.listener.on('connection', function(socket) {
    console.log('Connection!');
    socket.on('disconnect', function() {
      console.log('Disconnection!');
    });
  });

  this.engineClient = net.createConnection({ port: 5000 }, () => {
    console.log('Connected!');

    var payload = 'testy testy';
    var buffer = Buffer.allocUnsafe(8 + payload.length);
    nbo.htonl(buffer, 0, RequestType.CREATE_GAME_LOBBY);
    nbo.htonl(buffer, 4, payload.length);
    buffer.fill(payload, 8);

    this.engineClient.write(buffer, () => {
      console.log('Sent a request');
    });
    this.engineClient.write(buffer, () => {
      console.log('Sent a request');
    });
  });

  this.engineClient.on('data', (data) => {
    console.log(data.toString());
  });
}

module.exports = Backend;
