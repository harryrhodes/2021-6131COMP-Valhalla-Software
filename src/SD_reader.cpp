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

// String SDReader::readSettings()
// {
//   File settings = SD.open(file);
//   if (settings)
//   {
//     while(settings.available()){
//       String line = settings.readStringUntil('/r');
//       line.trim();
//       if (line.length() > 0){
//         return line;
//       }
//     }
//   }
//   else
//   {
//     Serial.println("Settings files not found - default values will be loaded");
//     return "Empty"; 
//   }
//   settings.close();
// }

String SDReader::readSettings()
{
	File min = SD.open(minSetting);
	File max = SD.open(maxSetting);

	if (min && max)
	{
		// String minLine = min.readStringUntil('/r');
		// String maxLine = max.readStringUntil('/r');
    int minTemp = atoi(min.readStringUntil('/r').c_str());
    int maxTemp = atoi(max.readStringUntil('/r').c_str());
		Serial.println("\nSETTINGS READ SUCCESSFULLY: Your starting Max temp is: " + String(maxTemp) + "C. Starting Min temp is: " + String(minTemp) + "C.\n");	
    // figure out how to return the min & max temp to   
  }
	else
	{
		Serial.println("Settings files not found - changing your min or max temp will build these files.");
	}
	min.close();
	max.close();
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
