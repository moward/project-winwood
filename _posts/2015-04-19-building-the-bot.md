---
layout: post
title: Building the Bot
---

## Learning SolidWorks

Our group initially used the Boe Bot for our line following robot but we needed a better chassis for our final setup so we designed one that fit our needs.  Our vehicles needed to be able to hold the Lidar module as well as the Raspberry Pi and any other components.  Neither one of us had previous experience laser cutting or using SolidWorks so we had to learn on the fly.
After experimenting with SolidWorks and gaining laser cutting certification, we started designing our robot and cutting the parts for assembly (see pictures below).

![Parts Picture](/images/SolidWorksParts.PNG)

## Printing the Pieces

After laser cutting our first set of parts and assembling them, we realized that the parts were too loose. We searched for the reason for this and found out the laser cutter cuts in a way that slightly alters the size of the materials from what they are defined as in SolidWorks. After researching press fits and the laser cutter's kerf (margin of error), we compensated for the error of the laser in the SolidWorks measurements and recut our vehicle's parts. The parts fit together properly and when the vehicle was assembled it was much more stable than before. 

![Assembled Parts](/images/BuiltChassis1.jpg)


## Setting Up the Lidar
Once we assembled our first vehicle, we turned our attention toward reading in the data from the Lidar. We attached the Lidar sensor to the Raspberry Pi pins so it could read the data as the Lidar spins.  After writing a program in C to read in the packets of data, we found that the packets we were receiving were not well formed and did not have the expected values. We occasionally received the start byte but not as often as expected and if we did receive it, the checksum bytes did not match with the data.  We spent a good amount of time trying to figure out what was happening and we decided that the most likely problem was dropping bits or misreading from serial. We checked the baud rate to verify it was set properly for the serial connection and we tried using different C functions to read in the data. After various troubleshooting we managed to get all the data (well-formed) and the checksum bytes verified that our packets were now being properly read.

## Continuing with the Lidar
We now plan on continuing our tests with the Lidar module and plan to use the data to determine the vehicles' positions and orientation. 