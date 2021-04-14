#include "User.h"
User::User(unsigned short minTemp, unsigned short maxTemp)
{
  this->minTemp = minTemp;
  this->maxTemp = maxTemp;
}

void User::setMinTemp(unsigned short v)
{
  if (v < maxTemp)
  {
    minTemp = v;
  }
}

void User::setMaxTemp(unsigned short v)
{
  if (v > minTemp)
  {
    maxTemp = v;
  }
}

void User::setStatus(UserState s)
{
  userStatus = s;
}

unsigned short User::getMinTemp()
{
  return minTemp;
}

unsigned short User::getMaxTemp()
{
  return maxTemp;
}

UserState User::getStatus()
{
  return userStatus;
}