#ifndef Reader_H_
#define Reader_H_
#include <vector>
#include <SD.h>
class Reader
{
    private:
    unsigned long lastWrite;
    const unsigned long interval = 120000; // 120000 = 2 mins, 1000 = 1s CHANGE ME TO LOWER WAIT TIME
    String currentValue;

    public:
    Reader();
    void tick(String temp, String date_time);
    void writeChanges(String date_time);
};
#endif