#ifndef SD_Reader_H_
#define SD_Reader_H_
#include <SD.h>
#include <string>
class SDReader
{
private:
  unsigned short chipSelect;
  char *minSetting;
  char *maxSetting;
  char *versionSetting;

public:
  SDReader(unsigned short pin, char *minSetting, char *maxSetting, char *versionSetting);
  bool init();
  std::vector<int> readSettings();
  void writeMinSettings(int minTemp);
  void writeMaxSettings(int maxTemp);
  String readVersion();
  void writeVersion(String version);
};
#endif