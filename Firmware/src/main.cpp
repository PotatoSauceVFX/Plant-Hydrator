#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "time.h"

// Pin definitions
#define PIN_LED 2

// WiFi settings
const char *ssid = "Sudo Pluto";
const char *password = "10200330";

// MQTT Settings
const char *mqtt_server = "0.0.0.0";

WiFiClient espClient;
PubSubClient client(espClient);

// Time settings
const char *ntpServer = "pool.ntp.org";
const int gmtOffset_hour = -5;
const int daylightOffset_hour = 1;
const char *dateTimeFormat = "%b %d %Y %I:%M:%S %p";

/*
  %a Abbreviated weekday name
  %A Full weekday name
  %b Abbreviated month name
  %B Full month name
  %c Date and time representation for your locale
  %d Day of month as a decimal number (01-31)
  %H Hour in 24-hour format (00-23)
  %I Hour in 12-hour format (01-12)
  %j Day of year as decimal number (001-366)
  %m Month as decimal number (01-12)
  %M Minute as decimal number (00-59)
  %p Current locale's A.M./P.M. indicator for 12-hour clock
  %S Second as decimal number (00-59)
  %U Week of year as decimal number,  Sunday as first day of week (00-51)
  %w Weekday as decimal number (0-6; Sunday is 0)
  %W Week of year as decimal number, Monday as first day of week (00-51)
  %x Date representation for current locale
  %X Time representation for current locale
  %y Year without century, as decimal number (00-99)
  %Y Year with century, as decimal number
  %z %Z Time-zone name or abbreviation, (no characters if time zone is unknown)
  %% Percent sign
  You can include text literals (such as spaces and colons) to make a neater display or for padding between adjoining columns.
  You can suppress the display of leading zeroes  by using the "#" character  (%#d, %#H, %#I, %#j, %#m, %#M, %#S, %#U, %#w, %#W, %#y, %#Y)
*/

void setup_local_time()
{
	//init and get the time
	Serial.println("Getting local time... ");
	configTime(gmtOffset_hour * 3600, daylightOffset_hour * 3600, ntpServer);
	Serial.println("Local time configured!");
}

void printCurrentLocalTime()
{
	struct tm timeinfo;

	if (!getLocalTime(&timeinfo))
	{
		Serial.println("Failed to obtain time");
		return;
	}

	Serial.println(&timeinfo, dateTimeFormat);
}

struct tm getCurrentLocalTime()
{
	struct tm datetime;
	if (!getLocalTime(&datetime))
	{
		Serial.println("Failed to obtain time");
	}

	return datetime;
}

void setup_wifi()
{
	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	unsigned long wifiBeginTime = millis();
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");

		if (millis() - wifiBeginTime > 10000)
		{
			Serial.println("\nWiFi Connect Failed, restarting in 3 seconds...");
			delay(3000);
			ESP.restart();
		}
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void setup()
{
	Serial.begin(115200);

	// LED Pin
	pinMode(PIN_LED, OUTPUT);

	// Connect to WiFi
	setup_wifi();

	// Init time
	setup_local_time();
}

void loop()
{
	//printCurrentLocalTime();
	struct tm datetime = getCurrentLocalTime();
	

	char timeStr[32];

	strftime(timeStr, 32, dateTimeFormat, &datetime);
	Serial.println(timeStr);

	delay(1000);
}