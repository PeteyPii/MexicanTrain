'use strict';

var http = require('http');

var express = require('express');

var Backend = require('./lib/backend');

var app = express();

app.use(function logRequests(req, res, next) {
  console.log('HIT');
  next();
});

var httpServer = http.createServer(app);
app.get('/', function(req, res) {
  res.redirect('/mexican-train');
});
var backend = new Backend(httpServer);

httpServer.listen(4000, function() {
  var host = httpServer.address().address;
  var port = httpServer.address().port;
  console.log('Server listening at http://' + host + ':' + port);
});
