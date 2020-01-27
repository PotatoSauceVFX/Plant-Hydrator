#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "time.h"
#include <Utilities.h>

// Pin definitions
#define PIN_LED 2

// WiFi settings
const char *ssid = "Sudo Pluto";
const char *password = "10200330";

// MQTT Settings
const char *mqtt_server = "192.168.137.1";
const int mqtt_port = 443;
const char *mqtt_clientId = "IsThisALongEnoughClientId?";
const char *mqtt_username = "SudoPluto";
const char *mqtt_password = "sdfjdoiusfe";

WiFiClientSecure espClient;
PubSubClient client(espClient);

const char CERT[] = R"======(
-----BEGIN CERTIFICATE-----
MIICZjCCAc+gAwIBAgIUVUvnXVMK/RH0lL91zZFdB7toDz4wDQYJKoZIhvcNAQEL
BQAwRTELMAkGA1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3RhdGUxITAfBgNVBAoM
GEludGVybmV0IFdpZGdpdHMgUHR5IEx0ZDAeFw0yMDAxMjcyMDMwMDdaFw0yNTAx
MjUyMDMwMDdaMEUxCzAJBgNVBAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEw
HwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQwgZ8wDQYJKoZIhvcNAQEB
BQADgY0AMIGJAoGBALTio15kYHiU7E8b+bx6uUmDbrkTfqjQUbCT1NgSerHJt6cV
xkkAealRAVcWOHc97062JDXEI51AQegWmU1hL2x2EstPcP38mou1xuY6vXp1WYG2
buuvy0v+BPNvkwoSJIKLUNTv5fNpvjq/qSsWbA1/B1h+bf4QMnX1FIIs0lV5AgMB
AAGjUzBRMB0GA1UdDgQWBBQmeNSLisu7HRNa1nVmwlkomF4qjzAfBgNVHSMEGDAW
gBQmeNSLisu7HRNa1nVmwlkomF4qjzAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3
DQEBCwUAA4GBAJ22xnbmgIuDqLk/Hj7hFerhK5SJfUfoLnniwZ3uWVSoIDt40RWT
HF4VPr61g6ujFc+pyZxQHQCnmroJU0qm+2WLuakJdR5ZCY20GWIDNRGNAuHfn0kN
mWsidSyQsccwbxqycuM+O9CzZzvUGpbhKmS396nbFob/ZDSRS4FOM9rJ
-----END CERTIFICATE-----

)======";


const char KEY[] = R"======(
-----BEGIN RSA PRIVATE KEY-----
MIICXAIBAAKBgQC04qNeZGB4lOxPG/m8erlJg265E36o0FGwk9TYEnqxybenFcZJ
AHmpUQFXFjh3Pe9OtiQ1xCOdQEHoFplNYS9sdhLLT3D9/JqLtcbmOr16dVmBtm7r
r8tL/gTzb5MKEiSCi1DU7+Xzab46v6krFmwNfwdYfm3+EDJ19RSCLNJVeQIDAQAB
AoGBAKDzEL6KvjRM/r+deue0o23AHyNbL1Zyll1yDO998eKERH/NpjOK0rZPt8uP
oAqp92eoNiwJLE9dv8cNZLcvhIuSbsBEUatbjIU1PAa1pRxJANsH3Iv+UvV/xOM2
t3wgv7NbKyXQtX0c8uDaTsrvc7jQ8+cw5MiROPNB+j1Nth6JAkEA8GPNm+FlCG6T
AXepsZetF7bRNpHoeNKRi1RrcDgg0+TTebcFjDbM1FG3dnDwAWL0a6y2KlmeWdKW
Ww5J42cKHwJBAMChpGvPhIaq4AQSJSWpY6CI1QD3kqaP17Mvpz9XUPy7lmbaWRQs
0gVlW6OM5nD+s4FG8taIogcybz9+0SpQXWcCQFi23FhctCaKeCIggTWTHkQUKd2T
t1FvlFrxCr1dWXAZkJnom1mFl837YlOJlmlMlZtvH8M9U50ehHKSMV6E1k0CQB9X
bXAZIH/kt3QSG7cn+0y4VqfEoDi3eKnv2+9jXorwXoU3i3zL2hQErMdMeoXx+vTS
vVoIewxeuVkEYjXXQ4sCQGQr8wDSZWDllITwmYwGRaQ7oc3JumDkJtCfqQy3sN2F
oVCu7pHovbN21LBwaUXsvYtgRiDio5cyPj352XMeJ8w=
-----END RSA PRIVATE KEY-----

)======";


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

void mqtt_callback(char *topic, byte *raw_message, unsigned int length)
{
	String message;

	String _topic = String(topic);

	for (int i = 0; i < length; i++)
	{
		message += (char)raw_message[i];
	}

	Serial.print("Message arrived on topic: ");
	Serial.print(topic);
	Serial.print(". Message: ");
	Serial.println(message);
}

void setup_mqtt()
{
	espClient.setCertificate(CERT);
	espClient.setPrivateKey(KEY);
	client.setServer(mqtt_server, mqtt_port);
	client.setCallback(mqtt_callback);
}

void reconnect_mqtt()
{
	// Loop until we're reconnected
	while (!client.connected())
	{
		Serial.println("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect(mqtt_clientId, mqtt_username, mqtt_password))
		{
			Serial.println("Connected!");
			// Subscribe
			//client.subscribe("esp32/output");
		}
		else
		{
			Serial.print("Failed to connect to MQTT Server, rc=");
			Serial.println(client.state());
			Serial.println("Trying again in 5 seconds...");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
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

	// MQtt
	setup_mqtt();
}

unsigned long lastClientPublish = millis();
void loop()
{
	if (!client.connected())
	{
		reconnect_mqtt();
	}
	client.loop();

	if (millis() - lastClientPublish > 1000)
	{
		struct tm datetime = getCurrentLocalTime();

		char timeStr[32];
		strftime(timeStr, 32, dateTimeFormat, &datetime);
		Serial.println(timeStr);
		client.publish("Time", timeStr);

		lastClientPublish = millis();
	}
}