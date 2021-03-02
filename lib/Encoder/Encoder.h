#include <ESP32Encoder.h>

class Encoder
{
private:
    int encodercnt = 0;
    ESP32Encoder encoder;

public:
    Encoder(int pinA, int pinB, int count);
    int rangeCheck(int min, int max);
};