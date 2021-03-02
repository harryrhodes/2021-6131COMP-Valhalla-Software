#include "Encoder.h"

Encoder::Encoder(int pinA, int pinB, int count)
{
  ESP32Encoder::useInternalWeakPullResistors = UP;
  encoder.attachHalfQuad(pinA, pinB);
  encoder.setCount(count);
}

// range checks the encode for temp
int Encoder::rangeCheck(int min, int max)
{
  if (encodercnt != encoder.getCount())
  {
    encodercnt = encoder.getCount();

    if (encodercnt >= max)
    {
      encodercnt = max;
      encoder.setCount(encodercnt);
    }
    else if (encodercnt <= min)
    {
      encodercnt = min;
      encoder.setCount(encodercnt);
    }
  }
  return encodercnt / 2;
}