#include "PIR_Sensor.h"
#include "User.h"
#include <Arduino.h>
PIRSensor::PIRSensor(unsigned short pin, unsigned short initTime)
{
  this->pin = pin;
  pinMode(this->pin, INPUT);
  this->initTime = initTime;
}

void PIRSensor::warmUp(unsigned short initTime, unsigned short warmUpTime)
{
  if (millis() >= initTime + warmUpTime)
  {
    ready = true;
    Serial.println("PIR Sensor Ready");
  }
}

bool PIRSensor::isReady()
{
  return ready;
}

UserState PIRSensor::read(int currentTime)
{
  int reading = digitalRead(pin);
  if (reading == HIGH)
  {
    lastMotionTime = millis();
    return UserState::PRESENT;
    }
  else if (reading == LOW)
  {
    if (currentTime >= lastMotionTime + timeout)
    {
      lastMotionTime = millis();
      return UserState::ABSENT;
    };
  }
}