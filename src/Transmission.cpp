#include "Transmission.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>
// #include "time.h"
// #include <ctime>

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

void setTimeFromServer()
{
    // initialise the time in UTC
    configTime(0, 3600, "pool.ntp.org");
}

char *getTime()
{
    // current date and time on the current system
    time_t now = time(0);
    // convert now to string form
    char *date_time = ctime(&now);

    return date_time;
}

void Transmission::sendStatus()
{
}