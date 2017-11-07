'use strict';

var http = require('http');

var express = require('express');

var addWebSocketListener = require('./lib/ws.js');

var app = express();

app.use(function logRequests(req, res, next) {
  console.log('HIT');
  next();
});

httpServer = http.createServer(app);
app.get('/', function(req, res) {
  res.redirect('/mexican-train');
});
addWebSocketListener(httpServer);

httpServer.listen(5555, function() {
  var host = httpServer.address().address;
  var port = httpServer.address().port;
  console.log('Server listening at http://' + host + ':' + port);
});
