#ifndef Reader_H_
#define Reader_H_
#include <vector>
#include <SD.h>
class Reader
{
    private:
    std::vector<std::string> list;
    String time;

    public:
    void logRecordings(std::vector<std::string>);
    String storeLog(String, std::vector<std::string>);
};
#endif