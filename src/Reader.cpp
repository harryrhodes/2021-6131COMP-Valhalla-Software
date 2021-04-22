#include "Reader.h"
#include <Arduino.h>
#include <SD.h>
#include <algorithm>
#include <string>

Reader::Reader()
{
    lastWrite = millis();
    currentValue = "";
}

void Reader::tick(String temp, String date_time) { // Adds temp to 1 long string, when interval time has passed, begin writing the string to SD Card
    currentValue = currentValue + temp + "\n";
    if (millis() > lastWrite + interval) {
        writeChanges(date_time);
    }
}

void Reader::writeChanges(String date_time) { // Makes new unique file, writes to SD card, cleanses string
    std::string formattedDT = std::string(date_time.c_str());
    std::replace(formattedDT.begin(), formattedDT.end(), ':', '_'); // replace all ':' to '_'
    std::replace(formattedDT.begin(), formattedDT.end(), '\n', '_'); // replace all newlines with '_'
    String fileName = "/" + String(formattedDT.c_str()) + ".txt";
    Serial.println(fileName);
    File file = SD.open(fileName, FILE_WRITE);
    file.println(currentValue);
    file.close();
    Serial.println("The recent recordings have been logged to the SD Card!");
    lastWrite = millis();
    currentValue = ""; // wipe the string
}