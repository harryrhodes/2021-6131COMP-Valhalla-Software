#include "RGB_LED.h"
#include <Arduino.h>

RGBLed::RGBLed(unsigned short redPin, unsigned short greenPin, unsigned short bluePin,
               unsigned short redChannel, unsigned short greenChannel, unsigned short blueChannel,
               unsigned short freq, unsigned short res)
{
  this->redPin = redPin;
  this->greenPin = greenPin;
  this->bluePin = bluePin;
  this->redChannel = redChannel;
  this->greenChannel = greenChannel;
  this->blueChannel = blueChannel;
  this->freq = freq;
  this->res = res;
}

void RGBLed::init()
{
  ledcSetup(redChannel, freq, res);
  ledcSetup(greenChannel, freq, res);
  ledcSetup(blueChannel, freq, res);
  ledcAttachPin(redPin, redChannel);
  ledcAttachPin(greenPin, greenChannel);
  ledcAttachPin(bluePin, blueChannel);
  setRGBValue(0, 0, 0);
}

void RGBLed::setRGBValue(unsigned short redValue, unsigned short greenValue,
                         unsigned short blueValue)
{
  ledcWrite(redChannel, redValue);
  ledcWrite(greenChannel, greenValue);
  ledcWrite(blueChannel, blueValue);
}
