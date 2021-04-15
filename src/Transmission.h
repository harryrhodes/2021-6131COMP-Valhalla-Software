#ifndef Transmission_H_
#define Transmission_H_
#include <vector>
#include <iostream>
#include <Arduino.h>

class Transmission
{
public:
    Transmission();
    bool sendReadings(std::vector<String> readings);
    void setTimeFromServer();
    char *getTime();
    void sendStatus();
};
#endif