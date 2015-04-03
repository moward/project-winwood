var b = require('bonescript');

// CONSTANTS
var leftWheel = 'P9_14'; //PWM1A
var rightWheel = 'P8_19'; //PWM2A
var irPin = ['P9_36', 'P9_38', 'P9_40'];
var DEBUG = true;

// TUNING PARAMETERS
var halfWheelDistance = 5.2; //half the distance between the wheels in centimeters
var forwardVelocity = 0.5; //forward speed as multiple of max distance
var trough_ms = 20; //specific for Parallax continuous rotation motors
var loopDelay = 500; //delay in milliseconds between iterations of main loop
var irThresholds = [0.13, 0.13, 0.16];
var directionIncrement = 3; //amount to change direction by per second

// GLOBAL VARIABLES
var direction = 0;

b.pinMode(leftWheel, b.OUTPUT);
b.pinMode(rightWheel, b.OUTPUT);

/* updateVelocity, change the PWM output on the given 
 * Inputs - string wheel, PWM pin assignment
 *        - number velocity, ranges from -1 to 1
 * Returns - nothing
 */

function updateDuty(wheel, velocity) {
    if (velocity < -1 || velocity > 1) {
        return;
    }
    // desired position in range 0..1
    var peak_ms = 1.5 + velocity * 0.2;
    var duty_cycle = peak_ms / (peak_ms + trough_ms);
    var freq = 1000 / (peak_ms + trough_ms);
    b.analogWrite(wheel, duty_cycle, freq, function(result) {
        if (DEBUG && result.err) {
            console.log('Error writing to PWM ' + wheel);
        }
    });
    if (DEBUG) console.log('Duty Cycle: '
        + parseFloat(duty_cycle*100).toFixed(1) + ' % on pin ' + wheel);
}

function main() {
    if (DEBUG) console.log('Main loop');
    
    var sensorBinaryReadings = [null, null, null];
    
    var irReadingCallback = function (reading, updateIndex) {
        if (DEBUG) console.log('IR Reading for index ' + updateIndex + ' is '
            + reading);
        sensorBinaryReadings[updateIndex] = reading < irThresholds[updateIndex];
        
        if (sensorBinaryReadings[0] != null
                && sensorBinaryReadings[1] != null
                && sensorBinaryReadings[2] != null) {
            if (DEBUG) console.log('sensorBinaryReadings: '
                + JSON.stringify(sensorBinaryReadings));
            
            //update direction
            var maxDelta = directionIncrement * loopDelay / 1000;
            
            if (!sensorBinaryReadings[0] && sensorBinaryReadings[1] && !sensorBinaryReadings[2]) {
                //straight
                
                //move closer to center
                if (direction > 0) {
                    direction = Math.max(0, direction - maxDelta);
                } else {
                    direction = Math.min(0, direction + maxDelta);
                }
            } else if (sensorBinaryReadings[0] && !sensorBinaryReadings[2]) {
                //left
                direction = Math.max(Math.min(1, direction - maxDelta), -1);
            } else if (!sensorBinaryReadings[0] && sensorBinaryReadings[2]) {
                //right
                direction = Math.max(Math.min(1, direction + maxDelta), -1);
            } else {
                direction = null;
                if (DEBUG) console.log('Unknown sensor readings');
            }

            if (DEBUG) console.log('Direction: ' + direction);
            
            //set wheel velocities
            //default to edge case of straight travel
            var leftVelocity = forwardVelocity;
            var rightVelocity = forwardVelocity;
            
            //handle general case
            if (direction != null && Math.abs(direction) > 0.05) {
                var radius = halfWheelDistance / Math.abs(direction);

                //make sure direction and radius have same sign
                if (direction < 0) {
                    radius *= -1;
                }

                var omega = forwardVelocity / radius;
                
                leftVelocity = omega * (radius - halfWheelDistance);
                rightVelocity = omega * (radius + halfWheelDistance);
            } else if (direction == null) {
                leftVelocity = rightVelocity = 0;
            }

            if (DEBUG) console.log('Velocities: (' + leftVelocity + ', '
                + rightVelocity + ')');
            
            updateDuty(leftWheel, leftVelocity);
            updateDuty(rightWheel, -rightVelocity);
            
            //set timeout to next call of loop
            setTimeout(main, loopDelay);
        }
    };
    
    for (var i = 0; i < 3; i++) {
        (function(i) {
            b.analogRead(irPin[i], function(reading) {
                irReadingCallback(reading.value, i);
            });
        })(i);
    }
    
}

main();
