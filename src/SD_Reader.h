#ifndef SD_Reader_H_
#define SD_Reader_H_
class SDReader
{
private:
  unsigned short chipSelect;
  char *file;

public:
  SDReader(unsigned short pin, char *file);
  void init();
  char readSettings();
  void writeSettings();
  void writeMinvalue(unsigned short v);
  void writeDebugLog(char, int currentTime);
};
#endif