'use strict';

var net = require('net');

var socketio = require('socket.io');
var grpc = require('grpc');

// var RequestType = require('../gen/RequestType_pb').RequestType;
var gameServerPb = grpc.load(__dirname + '/../../common/src/services/GameServer.proto');

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

  this.engineClient = new gameServerPb.GameServer('localhost:5000', grpc.credentials.createInsecure());
  var call = this.engineClient.sendAndReceive();

  call.on('end', function() {
    console.log('RPC call ended');
  });

  call.on('data', function(clientRequest) {
    console.log('Received client request:', clientRequest);
  });

  call.write({ test: 5 });
  call.write({ test: 4 });
  call.write({ test: 1 });
  call.write({ test: 2 });
  call.end();
}

module.exports = Backend;
