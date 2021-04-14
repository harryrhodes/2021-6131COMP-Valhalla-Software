#ifndef Reader_H_
#define Reader_H_
#include <vector>
#include <SD.h>
class Reader
{
    private:
    unsigned long lastWrite;
    const unsigned long interval = 10000; // 120000 = 2 mins, 1000 = 1s
    String currentValue;

    public:
    Reader();
    void tick(String temp);
    void writeChanges();
};
#endif