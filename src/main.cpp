#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library for the ST7735 screen
#include <Encoder.h>
#include <RGB_LED.h>
#include <User.h>
#include <PIR_Sensor.h>
#include <SD_Reader.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiType.h>
// #include <Reading.h>
#include <ctime>
#include <iostream>
#include <vector>
#include "time.h"
#include <ArduinoJson.h>

enum Demand
{
	HEAT,
	COOL,
	PASSIVE
};

enum Choice
{
	MENU,
	MIN,
	MAX
};

PIRSensor *pirSensor = NULL;
User *user = NULL;
RGBLed *led = NULL;
// Reading *readings = NULL;

DHT dht(26, DHT11);
Demand d = PASSIVE;
Choice c = MENU;
// rotary
const int ROTARY_A = 16;
const int ROTARY_B = 17;
const int ROTARY_BUTTON = 32;
unsigned long lastButtonPress = 0;
// sensor validation
bool SDAvailable = false;
int currentAirTemp = 0;
int oldMenuSelect = 0;
int currentMenuSelect = 0;

// setting delay values
unsigned long lastDebugTime;
int debugDelayValue = 5000;
unsigned long lastOrangeBlinkTime;
boolean orangeBlink = true;
int vacantDelayBlinkValue = 1000;
unsigned long lastVolatileReadingTime;
int volatileReadingDelayValue = 10000;
unsigned long lastTransmissionTime;
int transmissionDelayValue = 15000; // CHANGE TO 45000 BEFORE SUBMISSION

std::vector<String> readings;

//wi-fi settings
const char *ssid = "Robert's iPhone";
const char *password = "hello123";
boolean wifiConnected = false;

TFT_eSPI tft = TFT_eSPI();

Encoder encoderMenu(ROTARY_A, ROTARY_B, 2);
Encoder encoderTemp(ROTARY_A, ROTARY_B, 40);

boolean timeDiff(unsigned long start, int specifiedDelay)
{
	return (millis() - start >= specifiedDelay);
}

HTTPClient client;

void setup()
{
	Serial.begin(115200);
	pinMode(ROTARY_BUTTON, INPUT_PULLUP);
	pirSensor = new PIRSensor(25, millis());
	user = new User(20, 21);
	led = new RGBLed(14, 12, 13, 0, 1, 2, 5000, 8);
	led->init();
	// readings = new Reading();

	//Load user settings
	// SDReader *sd = new SDReader(5, "/settings");
	// if (sd->init())
	// {
	// 	sd->readSettings();
	// 	delete (sd);
	// }

	// initialise the screen
	dht.begin();
	tft.init();
	tft.setRotation(4);
	tft.fillScreen(TFT_BLACK);

	// connect to wi-fi
	WiFi.begin(ssid, password);

	// set timers
	lastDebugTime = millis();
	lastVolatileReadingTime = millis();
	lastTransmissionTime = millis();
}

void handleTempChange(int temp, UserState pirReading)
{
	if (temp <= user->getMaxTemp() && temp >= user->getMinTemp())
	{
		d = PASSIVE;
		if (pirReading == UserState::PRESENT)
			led->setRGBValue(0, 255, 0);
	}
	else if (temp > user->getMaxTemp())
	{
		d = COOL;
		if (pirReading == UserState::PRESENT)
			led->setRGBValue(0, 0, 255);
	}
	else if (temp < user->getMinTemp())
	{
		d = HEAT;
		if (pirReading == UserState::PRESENT)
			led->setRGBValue(255, 0, 0);
	}
}

void vacantBuldingBlink()
{
	if (timeDiff(lastOrangeBlinkTime, vacantDelayBlinkValue))
	{
		if (orangeBlink)
		{
			led->setRGBValue(0, 0, 0);
			orangeBlink = false;
		}
		else
		{
			led->setRGBValue(255, 135, 0);
			orangeBlink = true;
		}
		lastOrangeBlinkTime = millis();
	}
}

void handleSensorReadings(int currentAirTempReading, UserState pirReading)
{
	//temperature log
	if (timeDiff(lastDebugTime, debugDelayValue) || timeDiff(lastVolatileReadingTime, volatileReadingDelayValue) || currentAirTempReading != currentAirTemp || pirReading != user->getStatus())
	{
		handleTempChange(currentAirTempReading, pirReading);

		if (currentAirTempReading != currentAirTemp)
		{
			Serial.println("New air temperature: " + String(currentAirTempReading) + "C.");
		}

		if (pirReading != user->getStatus())
		{
			if (pirReading == UserState::PRESENT)
			{
				Serial.println("The Building state has changed to Occupied");
				user->setStatus(UserState::PRESENT);
			}
			else if (pirReading == UserState::ABSENT)
			{
				Serial.println("The Building State has changed to Vacant");
				user->setStatus(UserState::ABSENT);

				led->setRGBValue(255, 135, 0);
				lastOrangeBlinkTime = millis();
				orangeBlink = true;
			}
		}

		// 5 second debug console
		if (timeDiff(lastDebugTime, debugDelayValue))
		{
			Serial.print("Temperature sensor's current/latest value is " + String(currentAirTempReading) + "C.");
			Serial.print(" The User is ");
			Serial.println(user->getStatus() == UserState::PRESENT ? "Present." : "Absent.");
			// reset timer
			lastDebugTime = millis();
		}

		// store readings in volatile memory every 10 seconds
		if (timeDiff(lastVolatileReadingTime, volatileReadingDelayValue))
		{
			// current date and time on the current system
			time_t now = time(0);
			// convert now to string form
			char *date_time = ctime(&now);

			String reading = "Date: " + String(date_time) + "Temperature: " + String(currentAirTempReading) + "C, Building is: " + String(user->getStatus() == UserState::PRESENT ? "Occupied" : "Vacant") + ".\n";
			readings.push_back(reading);
			Serial.println(readings.size());
			// reset timer
			lastVolatileReadingTime = millis();
		}
	}

	if (pirReading != UserState::PRESENT)
	{
		vacantBuldingBlink();
	}
}

void checkButtonState()
{
	// Read the button state
	int btnState = digitalRead(ROTARY_BUTTON);
	//If we detect LOW signal, button is pressed
	if (btnState == LOW)
	{
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (timeDiff(lastButtonPress, 50))
		{
			if (c == MIN || c == MAX)
			{
				// log new change
				if (c == MIN)
				{
					Serial.println("New min temperature set to: " + String(user->getMinTemp()) + "C.");
					// writeSettings(user.getMinTemp(), user.getMaxTemp()); // save settings
				}
				else if (c == MAX)
				{
					Serial.println("New max temperature set to: " + String(user->getMaxTemp()) + "C.");
					// writeSettings(user.getMinTemp(), user.getMaxTemp()); // save settings
				}

				oldMenuSelect = 0;
				c = MENU;
			}
			else if (c == MENU && encoderMenu.rangeCheck(2, 4) == 1)
			{
				oldMenuSelect = 0;
				c = MIN;
			}
			else if (c == MENU && encoderMenu.rangeCheck(2, 4) == 2)
			{
				oldMenuSelect = 0;
				c = MAX;
			}
		}
		// Remember last button press event
		lastButtonPress = millis();
	}
}

void display(int currentAirTempReading)
{
	int minTempInput;
	int maxTempInput;
	if (c == MENU)
		currentMenuSelect = encoderMenu.rangeCheck(2, 4);
	if (c == MIN)
		minTempInput = encoderTemp.rangeCheck(20, 56);
	if (c == MAX)
		maxTempInput = encoderTemp.rangeCheck(20, 56);

	//only reset the display if there is a change
	if (currentAirTemp != currentAirTempReading || oldMenuSelect != currentMenuSelect || user->getMinTemp() != minTempInput || user->getMaxTemp() != maxTempInput)
	{
		tft.setCursor(0, 0, 2);
		//font to white and background to black
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
		tft.setTextSize(1);
		tft.println("Temperature: " + String(currentAirTempReading) + "C");
		tft.print("Min: ");
		if (c == MIN)
		{
			tft.setTextColor(TFT_BLACK, TFT_WHITE);
		};
		tft.print(String(user->getMinTemp()) + "C");
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
		tft.print(", Max: ");
		if (c == MAX)
		{
			tft.setTextColor(TFT_BLACK, TFT_WHITE);
		};
		tft.println(String(user->getMaxTemp()) + "C");
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
		if (user->getStatus() == UserState::PRESENT)
		{
			tft.println("User Is: Present");
		};
		if (user->getStatus() == UserState::ABSENT)
		{
			tft.println("User Is: Absent  ");
		};
		if (!pirSensor->isReady())
		{
			tft.println("PIR Sensor Loading");
		}
		else
		{
			tft.println("PIR Sensor Ready   ");
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
		user->setMinTemp(minTempInput);
		user->setMaxTemp(maxTempInput);
		currentAirTemp = currentAirTempReading;
	}
}

void handleHttpTransmission()
{
	if (wifiConnected && timeDiff(lastTransmissionTime, transmissionDelayValue))
	{
		//HTTP Client
		client.begin("192.168.56.1", 4000); // change IP accordingly

		client.addHeader("Content-Type", "application/json");

		// add readings to the payload as an array
		StaticJsonDocument<200> doc;
		JsonArray data = doc.createNestedArray("data");

		for (int i = 0; i < readings.size(); i++)
		{
			data.add(readings[i]);
		}

		String requestBody;
		serializeJson(doc, requestBody);

		int httpResponseCode = client.POST(requestBody);

		// check the response
		if (httpResponseCode > 0)
		{
			String response = client.getString();
			Serial.println(httpResponseCode);
			// if successful clear the memory
			if (httpResponseCode == 200)
			{
				readings.clear();
			}
		}
		else
		{
			Serial.println("Failed to send the readings to the server.");
		}
		// reset timer
		lastTransmissionTime = millis();
	}
}

void loop()
{
	if (WiFi.status() == WL_CONNECTED && !wifiConnected)
	{
		wifiConnected = true;
		Serial.println("Connected to the WiFi network.");
		// initialise the time in UTC
		configTime(0, 3600, "pool.ntp.org");
	}

	if (!pirSensor->isReady())
	{
		pirSensor->warmUp();
		vacantBuldingBlink();
	}
	else
	{
		int currentAirTempReading = dht.readTemperature();
		checkButtonState();														// #1
		handleSensorReadings(currentAirTempReading, pirSensor->read(millis())); // #2
		display(currentAirTempReading);											// #3
		handleHttpTransmission();												// #4
	}
}