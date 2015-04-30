/*
 *  closedLoop.h
 *  Header file for movement
 */

  //could also use this for making enum a type:
 //add whatever behaviors need to be kept track of or added
typedef enum {STOPSIGN, DESTINATION, INTERSECTION, NONE} behavior;  //(behavior is the type)

#ifndef POSITION
typedef struct position {
  double x;
  double y;
  double direction; //in degrees
} position;
#endif

typedef struct linkednode
{ 
  behavior behav;
  LINKEDNODE* next;
} LINKEDNODE;

//made coordinates doubles because thats what they are in position struct already
typedef struct node
{ 
  int cost;
  double direction;
  double xCoord;
  double yCoord;
  double speedLimit;
  LINKEDNODE* neighbors;
} NODE;

void closedLoopControlToNextPoint(position currPos, LINKEDNODE nextNode);

LINKEDNODE* planRoute(position currPos, float destX, float destY);
