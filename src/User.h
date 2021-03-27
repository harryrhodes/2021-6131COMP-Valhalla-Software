#ifndef User_H_
#define User_H_
enum class UserState
{
  PRESENT,
  ABSENT
};
class User
{
private:
  unsigned short minTemp;
  unsigned short maxTemp;
  UserState userStatus = UserState::ABSENT;

public:
  User(unsigned short minTemp, unsigned short maxTemp);
  void setMinTemp(unsigned short v);
  void setMaxTemp(unsigned short v);
  void setStatus(UserState s);
  unsigned short getMinTemp();
  unsigned short getMaxTemp();
  UserState getStatus();
};
#endif