#include "SD_Reader.h"
#include <Arduino.h>
#include <SD.h>

SDReader::SDReader(unsigned short chipSelect, char *minSetting, char *maxSetting)
{
  this->chipSelect = chipSelect;
  this->minSetting = minSetting;
  this->maxSetting = maxSetting;
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

std::vector<int> SDReader::readSettings()
{
	File min = SD.open(minSetting);
	File max = SD.open(maxSetting);
  std::vector<int> list;

	if (min && max){
    int minTemp = atoi(min.readStringUntil('\n').c_str());
    int maxTemp = atoi(max.readStringUntil('\n').c_str());
		Serial.println("SETTINGS READ SUCCESSFULLY: Your starting Max temp is: " + String(maxTemp) + "C. Starting Min temp is: " + String(minTemp) + "C.");	
    list.push_back(minTemp);
    list.push_back(maxTemp);
    min.close();
	  max.close();
  }
	else
	{
		Serial.println("Settings files not found - changing your min or max temp will build these files.");
	}
  return list;
}

void SDReader::writeMinSettings(int minTemp)
{
  File min = SD.open(minSetting, FILE_WRITE);
  if (min)
  {
    min.println(String(minTemp));
    Serial.println("Min temp overridden");
    min.close();
  }
  else
  {
    Serial.println("Couldn't open the min settings file to begin overwrite!");
  }
}

void SDReader::writeMaxSettings(int maxTemp)
{
  File max = SD.open(maxSetting, FILE_WRITE);
  if (max)
  {
    max.println(String(maxTemp));
    Serial.println("Max temp overridden");
    max.close();
  }
  else
  {
    Serial.println("Couldn't open the max settings file to begin overwrite!");
  }
}
