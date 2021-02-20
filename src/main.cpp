
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

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

//setting sensor pins
const int DHT_11_PIN = 26;

unsigned long lastChangeTime;
int delayValue = 1000;

DHT dht(DHT_11_PIN, DHT11);

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
	dht.begin();
}

int oldDuty;
boolean sensorReady = false;
Demand d = PASSIVE;
void handleTempChange(int temp)
{
	Serial.println(temp);
	if (temp == 25)
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
void loop()
{
	if (timeDiff(lastChangeTime, delayValue))
		handleTempChange(dht.readTemperature());

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