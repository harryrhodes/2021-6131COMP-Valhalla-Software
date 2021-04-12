#include "SD_Reader.h"
#include <Arduino.h>
#include <SD.h>

SDReader::SDReader(unsigned short chipSelect, char *file)
{
  this->chipSelect = chipSelect;
  this->file = file;
}

bool SDReader::init()
{
  SD.begin(chipSelect);
  if (!SD.begin(chipSelect))
  {
    Serial.println("The SD card mount has failed!");
    return false;
  }
  uint8_t currentCard = SD.cardType();
  if (currentCard == CARD_NONE)
  {
    Serial.println("No SD card found - no data will saved during this session!");
    return false;
  }
  Serial.println("SD Card initialisation starting");
  if (!SD.begin(chipSelect))
  {
    Serial.println("The card initialisation has failed!");
    return false;
  }
  return true;
}

String SDReader::readSettings()
{
  File settings = SD.open(file);
  if (settings)
  {
    while(settings.available()){
      String line = settings.readStringUntil('/r');
      line.trim();
      if (line.length() > 0){
        return line;
      }
    }
  }
  else
  {
    Serial.println("Settings files not found - default values will be loaded");
    return "Empty"; 
  }
  settings.close();
}

void SDReader::writeSettings()
{
  File settings = SD.open(file, FILE_WRITE);
  if (settings)
  {
    settings.println("Some Data");
    Serial.println("new temp saved to settings");
  }
  else
  {
    Serial.println("Couldn't open the settings file to begin overwrite!");
  }
  settings.close();
}
