var ioLib = require('socket.io');

var redis = require("redis");

module.exports = function(http) {
  var io = ioLib(http);

  var logSubscriber = redis.createClient();

  logSubscriber.on('message', function (channel, message) {
    //broadcast to all users
    io.emit('stdlog', message)
  });

  logSubscriber.subscribe('stdlog');

  io.on('connection', function(socket){
    console.log('a user connected');
    socket.on('disconnect', function(){
      console.log('user disconnected');
    });

    socket.on('stdin', function(input) {
      console.log('stdin: ' + input);
      socket.emit('stdout', 'You entered: ' + input + '\n');
    });
  });
}