/*
 *  closedLoop.h
 *  Header file for movement
 */

#define AT_DESTINATION_THRESH 20

extern pthread_mutex_t waypointListLock;

  //could also use this for making enum a type:
 //add whatever behaviors need to be kept track of or added
typedef enum {STOP_SIGN, DESTINATION, INTERSECTION, NONE} behavior;  //(behavior is the type)

struct linkednode;

//made coordinates doubles because thats what they are in position struct already
typedef struct node
{ 
  int cost;
  position pos;
  double speedLimit;
  struct linkednode* neighbors;
} NODE;

typedef struct linkednode
{ 
  behavior behav;
  NODE* node;
  struct linkednode* next;
} LINKEDNODE;

void closedLoopControlToNextPoint(position* currPos, LINKEDNODE* nextNode);

LINKEDNODE* planRoute(position* currPos, NODE* destination);
