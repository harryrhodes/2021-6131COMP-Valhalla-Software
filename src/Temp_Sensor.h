#ifndef Temp_Sensor_H_
#define Temp_Sensor_H_
class TempSensor
{
private:
  unsigned short pin;

public:
  TempSensor(unsigned short pin);
  void init();
  unsigned short read();
};
#endif