---
layout: post
title: Line-Following Robot
---

##Turning

We wanted to be able to command our robot to drive in a given direction as a result of line sensor input, thus implementing a closed loop system. Since the Boe-Bot's drivetrain consists of two coaxial motors and one dummy wheel, we needed a set of formulae that matched this specification. After a quick search online yielded no results, it was time to hit the white board.

In essence, we needed to determine the speed of each wheel as a function of the robot's velocity at the midpoint of the two wheels, which we denote as \\(v_f\\) and the direction, which we denote as \\(x \in [-1,1]\\). Here, \\(x=-1\\) means a hard left, where the left wheel is stationary, \\(x=0\\) means driving straight, and \\(x=1\\) means a hard right, where the right wheel is stationary. This continuous range of values makes it easier for us to adjust direction based upon inputs.

To be continued...

##References
 - [Beaglebone specs](http://beagleboard.org/support/bone101)
 - [Installing network-over-USB driver](http://beagleboard.org/getting-started)
 - [Enabling internet bridging](http://derekmolloy.ie/beaglebone/getting-started-usb-network-adapter-on-the-beaglebone/)
