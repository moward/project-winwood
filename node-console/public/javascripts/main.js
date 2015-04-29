/* Project winwood */
var convertToPre = function (input) {
  return input
      .replace(/\t/g,'    ')
      .replace(/\n/g,'<br/>')
      .replace(/\s/g, '&nbsp;');
}

var printToConsole = function (text, cssClass) {
  $('.console-output-inner').append('<span class="' + cssClass + '">' + convertToPre(text) + '</span>')
  $('.console-output').scrollTop($('.console-output-inner').height());
}

$(function() {
  //console stuff
  var socket = io();

  socket.on('stdout', function(out) {
    printToConsole(out, '');
  });
  socket.on('stdlog', function(out) {
    printToConsole(out, 'log');
  });
  $('.console-input').keyup(function (e) {
    if (e.keyCode == 13) {
      var input = $('.console-input').val();
      socket.emit('stdin', input);
      printToConsole(input + '\n', 'user');
      $(this).val('');
    }
  });

  //graph stuff
  var graphSize = {x: 600, y: 400};
  var scale = 0.5;
  var pointRadius = 6;
  var carSize = 175 * scale;

  var draw = SVG('graph');

  var drawPoint = function (x, y) {
    var coords = mapCoords(x, y);
    return draw.circle(pointRadius)
      .move(coords[0] - pointRadius / 2, coords[1] - pointRadius / 2)
      .attr({fill: '#FF4136'});
  }

  var mapCoords = function (x, y) {
    return [
      x * scale + graphSize.x / 2,
      -y * scale + graphSize.y / 2
    ];
  };

  var getGraphReading = function() {
    if ($('#graphToggle').prop('checked')) {
      if($('#graphType').val() === 'Lidar Readings') {
        $.post('/getLidarReading', {robotName: 'robo1'}, function (distances) {
          scale = 0.3;

          draw.clear();

          for (var i = 0; i < distances.length; i++) {
            if (distances[i] != -1) {
              var angle = i * Math.PI / 180;
              var x = distances[i] * Math.cos(angle);
              var y = distances[i] * Math.sin(angle);
              drawPoint(x, y);
              //console.log('data.addRow([' + x + ', ' + y + ']);');
            }
          }

          //run again after 250ms
          setTimeout(getGraphReading, 250);
        });
      } else if ($('#graphType').val() === 'Track Map') {
        $.post('/getPosition', {robotName: 'robo1'}, function (positions) {
          scale = 0.5;
          
          draw.clear();

          //draw outline
          draw.polygon([
            mapCoords(510, 360),
            mapCoords(-510, 360),
            mapCoords(-510, -360),
            mapCoords(510, -360)
          ]).fill('none').stroke({ color: '#39CCCC', opacity: 0.6, width: 2 });

          var roboNames = Object.getOwnPropertyNames(positions);

          for (var i = 0; i < roboNames.length; i++) {
            var carCoords = mapCoords(positions[roboNames[i]].x, positions[roboNames[i]].y);

            //draw cars
            var image = draw.image('/car.svg', carSize)
              .move(carCoords[0] - carSize / 2, carCoords[1] - carSize / 2)
              .rotate(-1 * positions[roboNames[i]].direction);
          }

          //run again after 250ms
          setTimeout(getGraphReading, 250);
        });
      }
    }
  };

  $('#graphToggle').click(getGraphReading);

  getGraphReading();
});
