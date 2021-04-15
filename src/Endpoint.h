#ifndef HTTPEndpoint_H_
#define HTTPEndpoint_H_
#include <vector>
#include <iostream>
#include <Arduino.h>

class Endpoint
{
private:
    char *host;
    unsigned int port;

public:
    Endpoint(char *host, unsigned int port);
    bool sendReadings(std::vector<String> readings);
    void setTimeFromServer();
    char *getTime();
    void sendStatus();
};
#endif