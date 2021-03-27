#ifndef RGBLed_H_
#define RGBLed_H_
class RGBLed
{
private:
  unsigned short redPin;
  unsigned short greenPin;
  unsigned short bluePin;
  unsigned short redChannel;
  unsigned short greenChannel;
  unsigned short blueChannel;
  unsigned short freq;
  unsigned short res;

public:
  RGBLed(unsigned short redPin, unsigned short greenPin, unsigned short bluePin,
         unsigned short redChannel, unsigned short greenChannel, unsigned short blueChannel,
         unsigned short freq, unsigned short res);
  void init();
  void setRGBValue(unsigned short redValue, unsigned short greenValue,
                   unsigned short blueValue);
};
#endif