'use strict';

var net = require('net');

var socketio = require('socket.io');
var grpc = require('grpc');
var crypto = require('crypto');

var settings = require('./settings');
var gameServerPb = grpc.load(__dirname + '/../../common/src/services/GameServer.proto');
var GameSettingsWrapper = require('../common/GameSettingsWrapper').GameSettingsWrapper;
var CreateGameRequest = require('../gen/CreateGameRequest_pb').CreateGameRequest;

function userHash(user) {
  var hmac = crypto.createHmac('sha256', settings.secret_key);
  hmac.update(user.name + '\0' + user.key);
  return hmac.digest();
}

function validateUser(user) {
  if (user.name.length > 128 || user.key.length > 128) {
    throw Error('User fields are too long.');
  }
  if (/[^\d\w-_]/.test(user.name)) {
    throw Error('User name has bad characters.');
  }
}

function Backend(frontendServer) {
  var self = this;
  self.listener = socketio(frontendServer, {
    path: '/mexican-train/socket.io',
  });

  self.listener.on('connection', function(socket) {
    console.log('Connection!');
    socket.on('create_game', function(data) {
      try {
        validateUser(data.user);
        var settings = new GameSettingsWrapper();
        settings.setNumberOfPlayers(data.gameSettings.numberOfPlayers);
        settings.setMaxPips(data.gameSettings.maxPips);
        settings.setStartingHandSize(data.gameSettings.startingHandSize);
        if (!settings.isValid()) {
          throw Error('Settings are not valid.');
        }
      } catch (err) {
        console.error('Failure processing "create_game".');
        console.error(data);
        console.error(err.stack);
        return;
      }

      var request = new CreateGameRequest();
      request.setUserHash(new Uint8Array(userHash(data.user)));
      request.setGameSettings(settings.getUnderlyingSettings());
      var wrappedRequest = new gameServerPb.ServerRequest();
      wrappedRequest.setRequestType(gameServerPb.ServerRequest.RequestType.CREATE_GAME);
      wrappedRequest.setRequestData(request.serializeBinary());
      self.engineClientChannel.write(wrappedRequest);
    });
    socket.on('disconnect', function() {
      console.log('Disconnection!');
    });
  });

  self.engineClient = new gameServerPb.GameServer('localhost:5000', grpc.credentials.createInsecure());
  self.createEngineClientChannel();
}

Backend.prototype.createEngineClientChannel = function createEngineClientChannel() {
  this.engineClientChannel = this.engineClient.sendAndReceive();
  this.engineClientChannel.on('end', function() {
    console.log('Restarting RPC call...');
    this.engineClientChannel.end();
    this.createEngineClientChannel();
  });

  this.engineClientChannel.on('data', function(clientRequest) {
    console.log('Received client request:', clientRequest);
  });
}

module.exports = Backend;
