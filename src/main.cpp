
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library for the ST7735 screen

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

// sensor validation
boolean sensorReady = false;
int currentAirTemp = 0;
int oldAirTemp = 0;

//setting sensor pins
const int DHT_11_PIN = 26;

unsigned long lastDebugTime;
int delayValue = 5000;

DHT dht(DHT_11_PIN, DHT11);

TFT_eSPI tft = TFT_eSPI();

enum Demand
{
	HEAT,
	COOL,
	PASSIVE
};

boolean timeDiff(unsigned long start, int specifiedDelay)
{
	return (millis() - start >= specifiedDelay);
}

void esp32Setup()
{
	// ledcAttachPin(LED_PIN, 1);
	// ledcSetup(1, 12000, 8);

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

	// initialise the screen
	tft.init();
	tft.setRotation(4);
	tft.fillScreen(TFT_BLACK);

	dht.begin();
}

Demand d = PASSIVE;
void handleTempChange(int temp)
{
	// Serial.print("Air temperature: ");
	// Serial.print(temp);
	// Serial.println("C.");

	if (temp == 22)
	{
		d = PASSIVE;
		ledcWrite(redChannel, 0);
		ledcWrite(greenChannel, 255);
		ledcWrite(blueChannel, 0);
	}
	else if (temp > 25)
	{
		d = COOL;
		ledcWrite(redChannel, 0);
		ledcWrite(greenChannel, 0);
		ledcWrite(blueChannel, 255);
	}
	else
	{
		d = HEAT;
		ledcWrite(redChannel, 255);
		ledcWrite(greenChannel, 0);
		ledcWrite(blueChannel, 0);
	}
}

void validateSensors(int temp)
{
	if (temp > -50 && temp < 60)
	{
		sensorReady = true;
		Serial.println("Temperature sensor is ready to use.\n");
	}
}

void debugLog(int currentTemp)
{
	if (timeDiff(lastDebugTime, delayValue) || oldAirTemp != currentTemp)
	{
		handleTempChange(currentTemp);

		if (oldAirTemp != currentTemp)
		{
			Serial.println("New air temperature: " + String(currentTemp) + "C.");
		}
		else
		{
			Serial.println("Temperature sensor's current/latest value is " + String(currentTemp) + "C.");
			// change last changed time here to rest it
			lastDebugTime = millis();
		}
		oldAirTemp = currentTemp;
	}
}

void display(int currentTemp)
{
	//only reset the display if there is a change
	if (oldAirTemp != currentTemp)
	{
		tft.setCursor(0, 0, 2);
		//font to white and background to black
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
		tft.setTextSize(1);
		tft.println("Temperature: " + String(currentTemp) + "C");
		tft.println("Min: 25C, Max: 25C");
		tft.println("OCCUPIED / VACANT");
		tft.println("");
		tft.println("Set Min Temp ");
		tft.println("Set Max Temp ");
	}
}

// void occupationDisplay()
// {
// }

void loop()
{
	currentAirTemp = dht.readTemperature();
	//first check if the sensor is ready to use and not reading it extreme values
	if (!sensorReady)
	{
		validateSensors(currentAirTemp);
	}
	else
	{
		display(currentAirTemp);
		// occupationDisplay();
		debugLog(currentAirTemp);
	}

	// int aRead = analogRead(DHT_11_PIN);
	// int duty = map(aRead, MIN_ANALOGUE, MAX_ANALOGUE, MIN_PWM_DUTY,
	// 							 MAX_PWM_DUTY);

	// if (oldDuty != duty && millis() % 500 == 0)
	// {
	// 	oldDuty = duty;
	// 	Serial.println(duty);

	// 	// check the sensor is working ok by
	// 	// seeing if the values are not min or max of the scale
	// 	if (duty != 0 && duty != 2147483647 && !sensorReady)
	// 	{
	// 		sensorReady = true;
	// 		Serial.println("Photoresistor sensor is ready.");
	// 	}
	// }
	// ledcWrite(1, duty);
}
