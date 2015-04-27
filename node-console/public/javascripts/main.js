/* Project winwood */
var data, chart, options;

var convertToPre = function (input) {
  return input
      .replace(/\t/g,'    ')
      .replace(/\n/g,'<br/>')
      .replace(/\s/g, '&nbsp;');
}

var printToConsole = function (text, cssClass) {
  $('.console-output').append('<span class="' + cssClass + '">' + convertToPre(text) + '</span>');
}

$(function() {
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

  var getLidarReading = function() {
    if ($('#graphToggle').prop('checked')) {
      $.post('/getLidarReading', {robotName: 'robo1'}, function (distances) {
        console.log(distances);

        //clear data
        data = new google.visualization.DataTable();

        data.addColumn('number');
        data.addColumn('number');

        for (var i = 0; i < distances.length; i++) {
          if (distances[i] != -1) {
            var angle = (90 - i) * Math.PI / 180;
            var x = distances[i] * Math.sin(angle);
            var y = distances[i] * Math.cos(angle);
            console.log('Adding '+ x + ',' + y);
            data.addRow([x, y]);
          }
        }

        //update chart
        chart.draw(data, options);

        //run again after 250ms
        setTimeout(getLidarReading, 250);
      })
    }
  };

  $('#graphToggle').click(getLidarReading)

  getLidarReading();
});

/* Graph stuff */
google.load("visualization", "1", {packages:["corechart"]});
google.setOnLoadCallback(drawChart);

function drawChart() {
  data = new google.visualization.DataTable();
  data.addColumn('number');
  data.addColumn('number');

  /*var radius = 100;
  for (var i = 0; i < 6.28; i += 0.1) {
    data.addRow([radius * Math.cos(i), radius * Math.sin(i)]);
  }

  // Our central point, which will jiggle.
  data.addRow([0, 0]);*/

  options = {
    legend: 'none',
    colors: ['#087037'],
    pointSize: 6,
    'chartArea': {'width': '90%', 'height': '80%'},
    animation: {
      duration: 0
    },
    /*'vAxis': {
      'minValue': -1000, 
      'maxValue': 1000},
    'hAxis': {
      'minValue': -1000, 
      'maxValue': 1000},*/
  };

  chart = new google.visualization.ScatterChart(document.getElementById('graph'));

  // Start the animation by listening to the first 'ready' event.
  //google.visualization.events.addOneTimeListener(chart, 'ready', randomWalk);

  // Control all other animations by listening to the 'animationfinish' event.
  //google.visualization.events.addListener(chart, 'animationfinish', randomWalk);

  chart.draw(data, options);

  /*function randomWalk() {
    var x = data.getValue(data.getNumberOfRows() - 1, 0);
    var y = data.getValue(data.getNumberOfRows() - 1, 1);
    x += 5 * (Math.random() - 0.5);
    y += 5 * (Math.random() - 0.5);
    if (x * x + y * y > radius * radius) {
      // Out of bounds. Bump toward center.
      x += Math.random() * ((x < 0) ? 5 : -5);
      y += Math.random() * ((y < 0) ? 5 : -5);
    }
    data.setValue(data.getNumberOfRows() - 1, 0, x);
    data.setValue(data.getNumberOfRows() - 1, 1, y);
    chart.draw(data, options);
  }*/
}