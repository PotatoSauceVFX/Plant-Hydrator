#include <Arduino.h>
#include <WiFi.h>

// WiFi settings
const char *ssid = "Sudo Pluto";
const char *password = "10200330";
const unsigned long WIFI_TIMEOUT = 10000;

void setup_wifi()
{
	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	unsigned long wifiBeginTime = millis();
    bool ledState = false;
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
        digitalWrite(PIN_LED, ledState);
        ledState = !ledState;

		if (millis() - wifiBeginTime > WIFI_TIMEOUT)
		{
			Serial.println("\nWiFi Connect Failed, restarting in 3 seconds...");
            digitalWrite(PIN_LED, LOW);
			delay(3000);
			ESP.restart();
		}
	}

    digitalWrite(PIN_LED, HIGH);
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
    Serial.println();
}