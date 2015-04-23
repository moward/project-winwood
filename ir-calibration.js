var irPin = ['P9_36', 'P9_38', 'P9_40'];
var b = require('bonescript');
b.analogRead(irPin[1], printStatus);
function printStatus(x) {
    console.log('x.value = ' + x.value);
}