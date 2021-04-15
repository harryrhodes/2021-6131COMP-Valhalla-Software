#ifndef Transmission_H_
#define Transmission_H_
#include <vector>

class Transmission
{
public:
    Transmission();
    bool sendReadings(std::vector<String> readings);
    void sendStatus();
};
#endif