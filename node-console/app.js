var express = require('express');
var app = express();
var path = require('path');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var http = require('http').Server(app);
var ioHandler = require('./socketHandler');
var redis = require('redis');

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

// uncomment after placing your favicon in /public
//app.use(favicon(__dirname + '/public/favicon.ico'));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

//send the lidar reading
app.post('/getLidarReading', function (req, res) {
  var redisHandler = redis.createClient();

  redisHandler.lrange(req.body.robotName + '.distances', 0, -1, function (err, data) {
    if (err) {
      console.log(err);
      res.send('error');
    } else {
      //return json object
      res.json(data);
    }
    redisHandler.quit();
  })
});

app.post('/getPosition', function (req, res) {
  var redisHandler = redis.createClient();

  redisHandler.scan(['0', 'match', '*.pos.*'], function (err, data) {
    if (err) {
      console.log(err);
      res.send('error');
    } else {
      var keys = data[1];
      redisHandler.mget(keys, function (err, data) {
        console.log(keys);
        console.log(data);
        if (err) {
          console.log(err);
          res.send('error');
        } else {
          var positions = {};
          for (var i = 0; i < keys.length; i++) {
            var key = keys[i];
            var keyParts = keys[i].split('.');
            positions[keyParts[0]] = positions[keyParts[0]] || {};
            positions[keyParts[0]][keyParts[2]] = data[i];
          }
          res.json(positions);
        }
      });
    }
  });
});

app.get('/', function(req, res) {
  res.render('index', {title: 'Project Winwood Web Console'});
});

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  res.send('<h1>404 File Not Found</h1>')
  //next(err);
});

ioHandler(http);

http.listen(3000, function() {
  console.log('listening on 3000');
});

// error handlers

// development error handler
// will print stacktrace
/*if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}*/