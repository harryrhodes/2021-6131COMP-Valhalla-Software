#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library for the ST7735 screen
#include <Encoder.h>

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
const int ROTARY_BUTTON = 5;
unsigned long lastButtonPress = 0;

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

Choice choice = MENU;

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

	if (temp <= maxTemp && temp >= minTemp)
	{
		d = PASSIVE;
		ledcWrite(redChannel, 0);
		ledcWrite(greenChannel, 255);
		ledcWrite(blueChannel, 0);
	}
	else if (temp > maxTemp)
	{
		d = COOL;
		ledcWrite(redChannel, 0);
		ledcWrite(greenChannel, 0);
		ledcWrite(blueChannel, 255);
	}
	else if (temp < minTemp)
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

void checkButtonState()
{
	// Read the button state
	int btnState = digitalRead(ROTARY_BUTTON);

	//If we detect LOW signal, button is pressed
	if (btnState == LOW)
	{
		Serial.println("Pressed");
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (timeDiff(lastButtonPress, 50))
		{
			if (choice == MIN || choice == MAX)
			{
				choice = MENU;
			}
			else if (choice == MENU && encoderMenu.rangeCheck(2, 4) == 1)
			{
				oldMenuSelect = 10;
				choice = MIN;
			}
			else if (choice == MENU && encoderMenu.rangeCheck(2, 4) == 2)
			{
				oldMenuSelect = 10;
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
	if (oldAirTemp != currentTemp || oldMenuSelect != currentMenuSelect || oldMinTemp != minTemp || oldMaxTemp != maxTemp)
	{
		tft.setCursor(0, 0, 2);
		//font to white and background to black
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
		tft.setTextSize(1);
		tft.println("Temperature: " + String(currentTemp) + "C");
		tft.println("Min: " + String(minTemp) + "C, Max: " + String(maxTemp) + "C");
		// tft.println("Vacant");
		tft.println("Occupied");
		tft.println("");
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
		checkButtonState();
		display(currentAirTemp);
		// occupationDisplay();
		debugLog(currentAirTemp);
	}
}
