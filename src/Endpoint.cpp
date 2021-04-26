#include "Endpoint.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>
#include <Update.h>

Endpoint::Endpoint(char *host)
{
    //Constructor
    this->host = host;
}

void Endpoint::setHost(char *host)
{
    this->host = host;
}

bool Endpoint::sendReadings(std::vector<String> readings)
{
    HTTPClient client;
    client.begin(host);
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

void Endpoint::sendStatus()
{
    // TODO: System health update for non-functional reliability testing
}

float Endpoint::getVersion()
{
    HTTPClient client;
    client.begin(host);
    int retCode = client.GET();

    if (retCode > 0)
    { //a real HTTP code
        Serial.print("HTTP " + String(retCode));
        if (retCode == HTTP_CODE_OK)
        {
            int contentLength = client.getSize();
            if (contentLength > 0)
            {
                return client.getString().toFloat();
            }
        }
    }
    else
    {
        Serial.println("Error... ");
        Serial.println(HTTPClient::errorToString(retCode));
    }
}

void Endpoint::getUpdate()
{
    HTTPClient client;
    client.begin(host);
    int retCode = client.GET();

    if (retCode > 0)
    { //a real HTTP code
        Serial.print("HTTP " + String(retCode));
        if (retCode == HTTP_CODE_OK)
        {
            int contentLength = client.getSize();
            if (contentLength <= 0)
            {
                Serial.println("Update size not specified");
                return;
            }
            if (!Update.begin(contentLength))
            {
                Serial.println("Not enough space to upgrade");
                return;
            }

            Serial.println("Getting Latest Update");
            int writeBytes = Update.writeStream(*client.getStreamPtr());
            if (writeBytes != contentLength)
            {
                Serial.println("only written " + String(writeBytes) + "bytes");
                Serial.println("expected " + String(contentLength) + "bytes");
                return;
            }
            if (!Update.end())
                Serial.println("Update Error Code" + String(Update.getError()));

            if (!Update.isFinished())
                Serial.println("Update failed.");
            else
            {
                Serial.println("Update finished: restarting");
                ESP.restart();
            }
        }
    }
    else
    {
        Serial.println("Error... ");
        Serial.println(HTTPClient::errorToString(retCode));
    }
}