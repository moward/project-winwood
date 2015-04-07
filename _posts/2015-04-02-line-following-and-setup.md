---
layout: post
title: Initial Setup & Line-Following
---

##Setup

We needed to set up our BeagleBone Black and test measuring inputs and setting up the PWM. To do this, we decided to create a line-following robot that used infrared to find and follow the line. We setup the BeagleBone and used a JS library called BlackLib to read from the analog pins and to use the PWM to power the motors for the wheels.  

##Line-Following

For the line-following circuit we used three IR emitters and three IR transistors. The circuit operats by emitting IR, which is then either reflected back by the white floor, or absorbed by the black line. The transistors are then either in saturation and result in a smaller voltage drop or they are in the triode region and there is a larger voltage drop. Based on the voltages after the transistors, we could determine which transistors were over the line and we could use the analog input pins to then direct the robot as needed. The circuit can be seen in the pictures below. 

![Front view of Line-Follower][PIC1]

[PIC1]: https://github.com/moward/project-winwood/blob/gh-pages/images/Line-follower-pic-1.jpg?raw=true "Front view of Line-Follower"

![Line-Follower][PIC2]

[PIC2]: https://github.com/moward/project-winwood/blob/gh-pages/images/Line-follower-pic-2.jpg?raw=true

##Demo

The video below shows the finished line-following robot on a small track.

^[VIDEO][]

^[http://www.youtube.com/embed/3UmC6acEHk0](YouTube)

[VIDEO]: https://www.youtube.com/watch?v=0VtZT9dBhiI