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
      var args = input.split(' ');
      if (args[0] === 'drive') {
        var commandPublisher = redis.createClient();
        var payload = args[1] + ',' + args[2];
        commandPublisher.publish('driveCommand', payload);
        socket.emit('stdout', 'Sent command: ' + payload + '\n');
        socket.emit('stdlog', 'Destination: ' + args[1] + ',' + args[2] + '\n');
      }
      //socket.emit('stdout', 'You entered: ' + input + '\n');
    });
  });
}