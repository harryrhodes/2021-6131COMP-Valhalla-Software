#include "Temp_Sensor.h"
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
TempSensor::TempSensor(unsigned short pin)
{
  this->pin = pin;
}

void TempSensor::init()
{
  DHT dht(this->pin, DHT11);
  dht.begin();
}

unsigned short TempSensor::read()
{
  return dht.readTemperature();
}
