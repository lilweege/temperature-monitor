#include "SFE_BMP180.h"
#include <Wire.h>

#include <WiFi.h>
#include <HTTPClient.h>

#define SDA_PIN 4
#define SCL_PIN 5

#define MEASUREMENT_INTERVAL_MS 60000  // One minute
SFE_BMP180 sensor;


// Define personal strings here, or in the build command

#ifndef WIFI_SSID
#error "WIFI_SSID must be defined!"
#endif
#ifndef WIFI_PASS
#error "WIFI_PASS must be defined!"
#endif
#ifndef SERVER_URL
#error "SERVER_URL must be defined!"
#endif
#ifndef SERVER_USER
#error "SERVER_USER must be defined!"
#endif
#ifndef SERVER_PASS
#error "SERVER_PASS must be defined!"
#endif


void setup()
{
    Serial.begin(9600);
    Serial.println("REBOOT");

    if (!sensor.begin(SDA_PIN, SCL_PIN)) {
        Serial.println("BMP180 init fail");
        while(1);
    }

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}


void loop()
{
    static unsigned long lastTime = 0;
    unsigned long now = millis();
    if (now - lastTime < MEASUREMENT_INTERVAL_MS)
        return;
    lastTime = now;


    char waitTimeMs = sensor.startTemperature();
    if (waitTimeMs == 0) return;
    delay(waitTimeMs);

    double temperature;
    char result = sensor.getTemperature(temperature);
    if (result != 0) {
        Serial.println(temperature, 2);

        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient http;
            http.begin(SERVER_URL);

            http.setAuthorization(SERVER_USER, SERVER_PASS);

            int httpResponseCode = http.POST(String(temperature, 2));

            if (httpResponseCode > 0) {
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);
                String payload = http.getString();
                Serial.println(payload);
            }
            else {
                Serial.print("Error code: ");
                Serial.println(httpResponseCode);
            }

            http.end();
        }
        else {
            Serial.println("WiFi Disconnected");
        }
    }
}
