---
layout: post
title: Final Demo Plan
---

##Overview

After our difficulties with the [initial setup of the microcontrollers and ROS](http://winwood.matt.how/2015/04/21/design-decisions.html), we decided to regroup and plan what we would do for our final demo this Friday. We decided that in order to focus on our original goals of the project, we would simplify positioning of the robots.

##Project Focus

The focus of this project is to have autonomous vehicles communicate their position, speed, and driving intentions to each other so that they can negotiate with each other to minimize traffic and driving time.

##The Demo

The lack of Lidar available and unsuccessful orders placed through Detkin Lab made obtaining Lidar difficult. Fortunately, we were able to obtain an extra one by ordering it ourselves. Therefore, we will have two vehicles in our world and we will demo various driving situations where they will interact and negotiate with each other.

##The Situations

* Situation 1: Both cars are approaching an intersection and they must negotiate to determine who goes through the intersection first as well as if a car needs to speed up or slow down
* Situation 2: Both cars are in the same lane and approach a stop sign. The cars should begin to slow down and come to a stop at roughly the same time
* Situation 3: If time permits, a third situation will be created where two cars attempt to merge into the same lane