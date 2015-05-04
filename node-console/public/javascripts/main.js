var destination = null;

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

  var positionExp = /([a-z0-9]+)\:\sRobot\sposition\:\s\((-?\d+\.\d\d),(\s-?\d+\.\d\d)\)\sAngle\:\s(\d+\.\d\d)/;
  var lidarExp = /([a-z0-9]+)\:\sLIDAR_STATS\:\s(\d+),\s(\d+)/;
  var destExp = /Destination\:\s(-?\d+\.\d\d),(-?\d+\.\d\d)/;

  socket.on('stdout', function(out) {
    printToConsole(out, '');
  });
  socket.on('stdlog', function(out) {
    printToConsole(out, 'log');
    //match for position data
    var posMatch = out.match(positionExp);
    var lidarMatch = out.match(lidarExp);
    var destMatch = out.match(destExp);
    if (posMatch) {
      $('#' + posMatch[1] + '_pos_x').html(posMatch[2]);
      $('#' + posMatch[1] + '_pos_y').html(posMatch[3]);
      $('#' + posMatch[1] + '_pos_direction').html(posMatch[4]);
    } else if (lidarMatch) {
      //match for LIDAR data
      $('#' + lidarMatch[1] + '_revolutionCount').html(lidarMatch[2]);
      $('#' + lidarMatch[1] + '_errorCount').html(lidarMatch[3]);
    } else if (destMatch) {
      destination = {x: destMatch[1], y: destMatch[2]};
    }
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
  var scale;
  var pointRadius = 6;

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
        $.post('/getLidarReading', {robotName: 'robo2'}, function (distances) {
          scale = 0.15;

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
          scale = 0.3;
          var carSize = 175 * scale;
          
          draw.clear();

          //draw outline
          draw.polygon([
            mapCoords(740, 490),
            mapCoords(-740, 490),
            mapCoords(-740, -490),
            mapCoords(740, -490)
          ]).fill('none').stroke({ color: '#39CCCC', opacity: 0.6, width: 2 });

          if (positions !== 'error') {
            var roboNames = Object.getOwnPropertyNames(positions);

            for (var i = 0; i < roboNames.length; i++) {
              var carCoords = mapCoords(positions[roboNames[i]].x, positions[roboNames[i]].y);

              //draw cars
              var image = draw.image('/car.svg', carSize)
                .move(carCoords[0] - carSize / 2, carCoords[1] - carSize / 2)
                .rotate(-1 * positions[roboNames[i]].direction);
            }

            if (destination) {
              //print destination
              drawPoint(destination.x, destination.y);
            }
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
