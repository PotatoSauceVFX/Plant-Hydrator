#include <Arduino.h>

// Pin definitions
#define PIN_LED 2

// Method definitions
void onCountUpdate(String jsonData);

#include <WiFi_Handler.h>
#include <NTP_Handler.h>
#include <MQTT_Handler.h>

// Global variable definitions
int GLOBAL_COUNT = 0;

void setup()
{
	Serial.begin(115200);

	// LED Pin
	pinMode(PIN_LED, OUTPUT);

	// Connect to WiFi
	setup_wifi();

	// Init time
	setup_local_time();

	// MQtt
	setup_mqtt();
}

unsigned long lastClientPublish = millis();
void loop()
{
	// Ensure MQTT is connected
	if (!client.connected())
	{
		reconnect_mqtt();
	}
	// Process MQTT client
	client.loop();
}

void onCountUpdate(String jsonData)
{
	StaticJsonDocument<200> doc;

	DeserializationError error = deserializeJson(doc, jsonData);

	// Test if parsing succeeds.
	if (error)
	{
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.c_str());
		return;
	}

	GLOBAL_COUNT = doc["Count"];
	const char* lastModified = doc["LastModified"];
	
	Serial.printf("New Count Data Recived: %i,\t Last modified %s\n", GLOBAL_COUNT, lastModified);
}