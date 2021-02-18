/**
 * This GPIO example applies hardware PWM to dim/brighten an
 * LED based on the value of an analogue input.
 *
 * Because analogWrite isn't implemented on the ESP32, this
 * approach uses the lower-level ESP32 helper functions for hardware PWM
 */
#include <Arduino.h>
#include <HardwareSerial.h>

const int MIN_ANALOGUE = 0;
const int MAX_ANALOGUE = 4095;
const int MAX_PWM_DUTY = 100;
const int MIN_PWM_DUTY = 0;

const int LED_PIN = 12;
const int RES_PIN = 26;

//sets up the ESP32's ADC on LED_PIN
// at a suitable resolution
void esp32Setup()
{
  ledcAttachPin(LED_PIN, 1);
  ledcSetup(1, 12000, 8);
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  esp32Setup();
}

// main loop method. This performs 3-4 discrete tasks:
// -samples the analogue pin
// -scales the reading based on the ADC resolution to a PWM 0-100 duty
// -logs this duty value every 500ms, whenever the duty changes
// -places the LED in the appropriate state, using ledcWrite (ESP32)
int oldDuty;
boolean sensorReady = false;
void loop()
{
  int aRead = analogRead(RES_PIN);
  int duty = map(aRead, MIN_ANALOGUE, MAX_ANALOGUE, MIN_PWM_DUTY,
                 MAX_PWM_DUTY);

  if (oldDuty != duty && millis() % 500 == 0)
  {
    oldDuty = duty;
    // Serial.print("led ");
    // Serial.println(duty);

    // check the sensor is working ok by
    // seeing if the values are not min or max of the scale
    if (duty != 0 && duty != 100 && !sensorReady)
    {
      sensorReady = true;
      Serial.println("Photoresistor sensor is ready.");
    }
  }
  ledcWrite(1, duty);
}
