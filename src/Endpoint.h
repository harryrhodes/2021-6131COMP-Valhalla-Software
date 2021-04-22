#ifndef HTTPEndpoint_H_
#define HTTPEndpoint_H_
#include <vector>
#include <iostream>
#include <Arduino.h>
#include <HTTPClient.h>
class Endpoint
{
private:
    char *host;
    unsigned int port;

public:
    Endpoint(char *host, unsigned int port);
    void setHost(char *host);
    bool sendReadings(std::vector<String> readings);
    void sendStatus();
    void getUpdate();
};
#endif