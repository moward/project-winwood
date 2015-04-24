//log an arbitrary string
//should end with a newline by convention
void redisLog(char *message);

//Post the robot's position to redis
void redisSetPosition(char *message);

//Post robot's reading
void redisPostReading(int* distances);

int openRedisConnection(char* serverIp, char* _robotName);
