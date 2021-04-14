#ifndef SD_Reader_H_
#define SD_Reader_H_
#include <SD.h>
class SDReader
{
private:
  unsigned short chipSelect;
  char *minSetting;
  char *maxSetting;

public:
  SDReader(unsigned short pin, char *file);
  bool init();
  String readSettings();
  void writeSettings();
  void writeMinvalue(unsigned short v);
  void writeDebugLog(char, int currentTime);
};
#endif