#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// MQTT Settings
const char *mqtt_server = "192.168.137.1";
const int mqtt_port = 443;
const char *mqtt_clientId = "Client-0";
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