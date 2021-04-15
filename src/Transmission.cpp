#include "Transmission.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>

Transmission::Transmission()
{
}

bool Transmission::sendReadings(std::vector<String> readings)
{
    //HTTP Client
    HTTPClient client;
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
        if (httpResponseCode == 200)
        {
            return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}

void Transmission::sendStatus()
{
}