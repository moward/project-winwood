---
layout: post
title: Project Proposal
---

##Team:
- Matthew Howard, lead on software modelling and wireless comm.
- Chris Champagne, lead on sensing and actuation

##Motivation:
Automotive traffic congestion leads to inefficient use of transportation infrastructure, unnecessary energy costs, increased greenhouse gas emissions, and increased travel times. These problems also lead to secondary yet significant effects including the need for additional highways and lanes, magnifying traffic congestion’s environmental and economic footprint. We posit that much of this congestion can be eliminated by automating both driving and the exchange of information between vehicles sharing the road.

Most research into self-driving vehicles today focuses on systems that exist within a landscape filled with human drivers. Because these systems are bound by the conventions and rules of engagement of human drivers, they can only be slightly more efficient than human drivers in using the space of the road.

##Goal of the project:
To produce a set of multiple autonomous vehicles that are able to collectively make decisions in order to greatly improve travel time and utilization of infrastructure.

##Methodology:
The key components of each autonomous vehicle are the drivetrain, the navigation system, the communication system, and the driving logic. While these concepts are all tied together, they can be examined separately to break down the design and development processes.

With the drivetrain, the first step is to develop a basic chassis that can support our other hardware. We expect this to be an iterative process; first, we will use a COTS or simple kit drivetrain as a proof-of-concept and to test bench our control systems. Then, based off our findings we plan to move to a more customized chassis and drivetrain that support our navigation system and can drive within the scale of our model world.

The most uncertain of these concepts is the navigation system since there are many possible solutions but no obvious one. For our first milestone, we will implement a line-following vehicle using a dark line on a light track, IR LEDs, and IR sensors. After that, we plan on using Neato LIDAR modules for absolution positioning in a world learned by an industry-grade LIDAR on a separate target vehicle. However, we are not committed to using LIDAR in the end and may experiment with other methods such as a road surface encoded with magnetic or optical information.

We plan on using wireless communication in order for the vehicles to communicate with each other and with the base station. One possibility is 802.11, since it is cheap, easy, fast, and integrates seamlessly with a laptop’s existing wireless chip. Another possibility is 802.15 (ZigBee) modules since they give us much finer control over how our information is directly encoded, which may make tasks like clock synchronization much more precise.

The driving logic is the last and most complicated part of the project. Our vehicles must not only be able to drive autonomously; they must also be able to negotiate decision making between each other. Once the vehicles are somewhat able to drive autonomously on their own, we plan on creating isolated traffic scenarios such as braking and accelerating in a line, meeting at an intersection, and merging lanes. Once these scenarios are handled in isolation, the vehicles will recognize and respond to these events as they come up naturally.

##Project Components:
* Several (3-4) autonomous vehicles, identical in specification, each including:
    + A drivetrain
    + Beagle Board running Ubuntu and ROS
    + IR LED and sensors for line following
    + LIDAR module
    + battery
    + Motor driver
    + Wifi module / Zigbee
* A base station (laptop) w/ GUI Control Panel
* Model world, i.e. a miniature driving environment

##Testing and Evaluation:
We will design and implement use cases, such as the ones described above and test our vehicles for reliability, efficiency, and safety. We will measure reliability by performing the same use case repeatedly with different initial configurations. We will measure efficiency as the time it takes for a vehicle to get to its destination (travel time), energy consumption (current draw or the integral of voltage applied) and required road distance per vehicle (for the driving in a line scenario). We will quantify safety as the minimum distance between two vehicles and between vehicles and obstacles.

##Deliverables:
**Milestone (Week) 1:** Create a line-following vehicle using the beagleboard and ROS that is able to navigate to given waypoints on a multi-lane track *- April 2nd*

**Milestone (Week) 2:** Create a set of vehicles that is able to navigate in a model world using environmental sensory information (LIDAR, hall-effect coded environment, or other) *- April 9th*

**Milestone (Week) 3:** Extend vehicles to drive together and to wirelessly negotiate on several traffic scenarios *- April 16th*

**Reach:** Implement P2P, decentralized communication between the vehicles *- April 23rd*