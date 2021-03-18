#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library for the ST7735 screen
#include <Encoder.h>
#include <SD.h>

// Analogue Converting Commented Out
// const int MIN_ANALOGUE = 0;
// const int MAX_ANALOGUE = 4095;
// const int MAX_PWM_DUTY = 255;
// const int MIN_PWM_DUTY = 0;

// const int LED_PIN = 12;

const int RED_PIN = 14;
const int GREEN_PIN = 12;
const int BLUE_PIN = 13;

// setting PWM properties
const int freq = 5000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
const int resolution = 8;

// rotary
const int ROTARY_A = 16;
const int ROTARY_B = 17;
const int ROTARY_BUTTON = 32; // I WAS 5 !!
unsigned long lastButtonPress = 0;

//motion sensor
const int MOTION_SENSOR = 25;
int pirReading = 0;

//const int currentState = 0;
//Timer
unsigned long sensorCheck;
unsigned long warmUp = 60000;
unsigned long loadingTime;
unsigned long motionSensorTime;
unsigned long currentTime;
unsigned long startingTime;
unsigned long targetTime;
unsigned long timeout = 200;
boolean startTimer = false;
boolean motionSensorReady = false;
boolean pirFunctionComplete = false;

// sensor validation
boolean sensorReady = false;
int currentAirTemp = 0;

int minTemp = 20;
int maxTemp = 20;

int oldAirTemp = 0;
int oldMenuSelect = 0;
int currentMenuSelect = 0;
int oldMinTemp = 20;
int oldMaxTemp = 20;

//setting sensor pins
const int DHT_11_PIN = 26;

unsigned long lastDebugTime;
int delayValue = 5000;

DHT dht(DHT_11_PIN, DHT11);

TFT_eSPI tft = TFT_eSPI();

Encoder encoderMenu(ROTARY_A, ROTARY_B, 2);
Encoder encoderTemp(ROTARY_A, ROTARY_B, 40);

// SD Card & Reader
const int chipSelect = 5;
const char *minSettings = "/minSettings.txt";
const char *maxSettings = "/maxSettings.txt";

void getSettings() // Get saved settings from the sd card
{
	File min = SD.open(minSettings);
	File max = SD.open(maxSettings);

	String minLine = min.readStringUntil('/r');
	minTemp = atoi(minLine.c_str());
	Serial.println("Min setting got & set to " + String(minTemp));
	String maxLine = max.readStringUntil('/r');
	maxTemp = atoi(maxLine.c_str());
	Serial.println("Max setting got & set to " + String(maxTemp));
	min.close();
	max.close();
}

void overwriteSettings(int minTemp, int maxTemp) // alter the saved settings for the temp
{
	File min = SD.open(minSettings, FILE_WRITE);
	File max = SD.open(maxSettings, FILE_WRITE);
	if (min && max)
	{
		min.println(String(minTemp));
		max.println(String(maxTemp));
		Serial.println("new temp saved to settings");
	}
	else
	{
		Serial.println("Couldn't open the settings file to begin overwrite!");
		return;
	}
	min.close();
	max.close();
}

enum Demand
{
	HEAT,
	COOL,
	PASSIVE
};

enum BuildingState
{
	OCCUPIED,
	VACANT
};

enum Choice
{
	MENU,
	MIN,
	MAX
};

Choice choice = MENU;
BuildingState buildingState = VACANT;
BuildingState oldState = VACANT;

boolean timeDiff(unsigned long start, int specifiedDelay)
{
	return (millis() - start >= specifiedDelay);
}

void esp32Setup()
{
	ledcSetup(redChannel, freq, resolution);
	ledcSetup(greenChannel, freq, resolution);
	ledcSetup(blueChannel, freq, resolution);
	ledcAttachPin(RED_PIN, redChannel);
	ledcAttachPin(GREEN_PIN, greenChannel);
	ledcAttachPin(BLUE_PIN, blueChannel);
}

void setup()
{
	Serial.begin(115200);
	// pinMode(LED_PIN, OUTPUT);
	esp32Setup();
	pinMode(ROTARY_BUTTON, INPUT_PULLUP);
	pinMode(MOTION_SENSOR, INPUT);

	// initialise the screen
	tft.init();
	tft.setRotation(4);
	tft.fillScreen(TFT_BLACK);

	dht.begin();

	SD.begin(chipSelect);
	if (!SD.begin(chipSelect))
	{
		Serial.println("The SD card mount has failed!");
		return;
	}
	uint8_t currentCard = SD.cardType();
	if (currentCard == CARD_NONE)
	{
		Serial.println("No SD card currently present - no data will saved during this session!");
		return;
	}
	Serial.println("SD Card initialisation starting");
	if (!SD.begin(chipSelect))
	{
		Serial.println("The card initialisation has failed!");
		return;
	}
	getSettings();
}

void vacancyAlert(BuildingState buildingState)
{
	ledcWrite(redChannel, 255);
	ledcWrite(greenChannel, 135);
	ledcWrite(blueChannel, 0);
}

Demand d = PASSIVE;
void handleTempChange(int temp)
{
	if (temp <= maxTemp && temp >= minTemp)
	{
		d = PASSIVE;
		if (buildingState == OCCUPIED)
		{
			ledcWrite(redChannel, 0);
			ledcWrite(greenChannel, 255);
			ledcWrite(blueChannel, 0);
		}
	}
	else if (temp > maxTemp)
	{
		d = COOL;
		if (buildingState == OCCUPIED)
		{
			ledcWrite(redChannel, 0);
			ledcWrite(greenChannel, 0);
			ledcWrite(blueChannel, 255);
		}
	}
	else if (temp < minTemp)
	{
		d = HEAT;
		if (buildingState == OCCUPIED)
		{
			ledcWrite(redChannel, 255);
			ledcWrite(greenChannel, 0);
			ledcWrite(blueChannel, 0);
		}
	}
}

void validateSensors(int temp)
{
	if (temp > -50 && temp < 60)
	{
		sensorReady = true;
		Serial.println("Temperature sensor is ready to use");
		Serial.println("PIR Sensor Warming Up\n");
		loadingTime = millis();
		motionSensorTime = loadingTime + warmUp;
	}
	else
	{
		Serial.println("Sensor Error");
	};
}

void pirWarmUp()
{
	currentTime = millis();
	if (currentTime >= motionSensorTime)
	{
		pirFunctionComplete = true;
		Serial.println();
		Serial.println("PIR Sensor Ready");
		Serial.println();
	}
	else
	{
		buildingState = VACANT;
	}
}

void debugLog(int currentTemp)
{

	if (buildingState == VACANT)
	{
		vacancyAlert(buildingState);
	}
	//temperature log

	if (timeDiff(lastDebugTime, delayValue) || oldAirTemp != currentTemp || oldState != buildingState)
	{
		handleTempChange(currentTemp);

		if (oldAirTemp != currentTemp)
		{
			Serial.println("New air temperature: " + String(currentTemp) + "C.");
		}
		else if (oldState != buildingState)
		{
			if (buildingState == OCCUPIED)
			{
				Serial.println("The Building state has changed to Occupied");
			}
			else
			{
				Serial.println("The Building State has changed to Vacant");
			}
		}
		else if (timeDiff(lastDebugTime, delayValue))
		{
			Serial.print("Temperature sensor's current/latest value is " + String(currentTemp) + "C.");
			Serial.print(" The Current Building State is ");
			Serial.println(buildingState == OCCUPIED ? "Occupied." : "Vacant.");

			// change last changed time here to rest it
			lastDebugTime = millis();
		}
	}
}

void checkVacancy()
{
	if (pirFunctionComplete == true)
	{
		pirReading = digitalRead(MOTION_SENSOR);
		if (pirReading == HIGH)
		{
			buildingState = OCCUPIED;
			//	Serial.println("Building is Occupied");
			sensorCheck = 0;
			startingTime = millis();
			targetTime = startingTime + timeout;
		}
		else if (pirReading == LOW)
		{
			sensorCheck = millis();
			if (sensorCheck >= targetTime)
			{
				buildingState = VACANT;
				//		Serial.println("Building is Empty");
			};
		}
		else
		{
			Serial.println("Error");
		}
	}
	else
	{
	}

	digitalWrite(MOTION_SENSOR, LOW);
}

void checkButtonState()
{
	// Read the button state
	int btnState = digitalRead(ROTARY_BUTTON);

	//If we detect LOW signal, button is pressed
	if (btnState == LOW)
	{
		// Serial.println("Pressed");
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (timeDiff(lastButtonPress, 50))
		{
			if (choice == MIN || choice == MAX)
			{
				// log new change
				if (choice == MIN)
				{
					Serial.println("New min temperature set to: " + String(minTemp) + "C.");
					overwriteSettings(minTemp, maxTemp); // save settings
				}
				else if (choice == MAX)
				{
					Serial.println("New max temperature set to: " + String(maxTemp) + "C.");
					overwriteSettings(minTemp, maxTemp); // save settings
				}

				oldMenuSelect = 0;
				choice = MENU;
			}
			else if (choice == MENU && encoderMenu.rangeCheck(2, 4) == 1)
			{
				oldMenuSelect = 0;
				choice = MIN;
			}
			else if (choice == MENU && encoderMenu.rangeCheck(2, 4) == 2)
			{
				oldMenuSelect = 0;
				choice = MAX;
			}
		}
		// Remember last button press event
		lastButtonPress = millis();
	}
}

void display(int currentTemp)
{
	if (choice == MENU)
		currentMenuSelect = encoderMenu.rangeCheck(2, 4);
	if (choice == MIN)
		minTemp = encoderTemp.rangeCheck(20, 56);
	if (choice == MAX)
		maxTemp = encoderTemp.rangeCheck(20, 56);

	//only reset the display if there is a change
	if (oldAirTemp != currentTemp || oldMenuSelect != currentMenuSelect || oldMinTemp != minTemp || oldMaxTemp != maxTemp || buildingState != oldState)
	{
		tft.setCursor(0, 0, 2);
		//font to white and background to black
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
		tft.setTextSize(1);
		tft.println("Temperature: " + String(currentTemp) + "C");
		tft.print("Min: ");
		if (choice == MIN)
		{
			tft.setTextColor(TFT_BLACK, TFT_WHITE);
		};
		tft.print(String(minTemp) + "C");
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
		tft.print(", Max: ");
		if (choice == MAX)
		{
			tft.setTextColor(TFT_BLACK, TFT_WHITE);
		};
		tft.println(String(maxTemp) + "C");
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
		if (buildingState == OCCUPIED && pirFunctionComplete == true)
		{
			tft.println("Building: Occupied  ");
			tft.println("");
			oldState = OCCUPIED;
		};
		if (buildingState == VACANT && pirFunctionComplete == true)
		{

			tft.println("Building: Vacant    ");
			tft.println("");
			oldState = VACANT;
		};
		if (!pirFunctionComplete)
		{
			tft.println("PIR Sensor Loading");
			tft.println("");
		};
		tft.print("Set Min Temp ");
		if (currentMenuSelect == 1)
		{
			tft.println("<-");
		}
		else
		{
			tft.println("  ");
		}
		tft.print("Set Max Temp ");
		if (currentMenuSelect == 2)
		{
			tft.println("<-");
		}
		else
		{
			tft.println("  ");
		}

		oldMenuSelect = currentMenuSelect;
		oldMinTemp = minTemp;
		oldMaxTemp = maxTemp;
		oldAirTemp = currentTemp;
	}
}

void loop()
{
	currentAirTemp = dht.readTemperature();
	//first check if the sensor is ready to use and not reading it extreme values
	if (!sensorReady)
	{
		validateSensors(currentAirTemp);
	}
	else if (!pirFunctionComplete)
	{
		pirWarmUp();
		checkButtonState();		  // #1
		checkVacancy();			  // #2
		debugLog(currentAirTemp); // #3
		display(currentAirTemp);  // #4
	}
	else
	{
		checkButtonState();		  // #1
		checkVacancy();			  // #2
		debugLog(currentAirTemp); // #3
		display(currentAirTemp);  // #4
	}
}
