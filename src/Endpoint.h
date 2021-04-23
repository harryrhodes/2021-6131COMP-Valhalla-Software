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

public:
    Endpoint(char *host);
    void setHost(char *host);
    bool sendReadings(std::vector<String> readings);
    void sendStatus();
    float getVersion();
    void getUpdate();
};
#endif