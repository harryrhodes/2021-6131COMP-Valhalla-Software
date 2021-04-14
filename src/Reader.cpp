#include "Reader.h"
#include <Arduino.h>
#include <SD.h>

Reader::Reader()
{
    lastWrite = millis();
    currentValue = "";
}

void Reader::tick(String temp) { // Adds temp to 1 long string, when interval time has passed, begin writing the string to SD Card
    currentValue = currentValue + temp + "\n";
    if (millis() > lastWrite + interval) {
        writeChanges();
    }
}

void Reader::writeChanges() { // Makes new unique file, writes to SD card, cleanses string
    String fileName = "/TEST_" + String(lastWrite) + ".txt"; // EPOCH TIME NEEDED
    File file = SD.open(fileName.c_str(), FILE_WRITE);
    file.println(currentValue);
    file.close();
    Serial.println("The recent recordings have been logged to the SD Card!");
    lastWrite = millis();
    currentValue = ""; // wipe the string
}