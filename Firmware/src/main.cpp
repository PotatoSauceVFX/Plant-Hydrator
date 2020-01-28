#include <Arduino.h>

// Pin definitions
#define PIN_LED 2

#include <WiFi_Handler.h>
#include <NTP_Handler.h>
#include <MQTT_Handler.h>

// Hardware timer
volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer()
{
	portENTER_CRITICAL_ISR(&timerMux);
	interruptCounter++;
	portEXIT_CRITICAL_ISR(&timerMux);
}

void setup()
{
	Serial.begin(115200);

	// Timer init
	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 500000, true);
	timerAlarmEnable(timer);

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

	while(digitalRead(0) == HIGH)
	{

	}

	// Handle timer interrupt
	if (interruptCounter > 0)
	{
		portENTER_CRITICAL(&timerMux);
		interruptCounter--;
		portEXIT_CRITICAL(&timerMux);

		totalInterruptCounter++;

		Serial.print("An interrupt as occurred. Total number: ");
		Serial.println(totalInterruptCounter);
		Serial.println();

		struct tm datetime = getCurrentLocalTime();

		char timeStr[32];
		strftime(timeStr, 32, dateTimeFormat, &datetime);
		Serial.println(timeStr);
		client.publish("Time", timeStr);
	}
}