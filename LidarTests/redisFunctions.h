//log an arbitrary string
//should end with a newline by convention
void redisLog(char *message);

//Post the robot's position to redis
void redisSetPosition(position *currPosition);

//Post robot's reading, include length of array
void redisPostReading(int* distances, int length);

int openRedisConnection(char* serverIp, char* _robotName);

void redisSubscribe(void (*fun)(redisReply*));
