#ifndef PIR_Sensor_H_
#define PIR_Sensor_H_
#include <User.h>
class PIRSensor
{
private:
  unsigned short pin;
  unsigned short initTime;
  unsigned short warmUpTime = 15000;
  int timeout = 5000; // CHANGE THIS TO 10 MINUTES BEFORE SUBMISSION!!! - 600000
  unsigned short lastMotionTime;
  bool ready = false;

public:
  PIRSensor(unsigned short pin, unsigned short initTime);
  void warmUp();
  bool isReady();
  UserState read(int currentTime);
};
#endif