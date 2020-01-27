#include <Arduino.h>
#include "time.h"

String parseTime(struct tm datetime, const char* format, int buffer = 128)
{
    String parsedString;

    char* timeStr = "";
	strftime(timeStr, buffer, format, &datetime);

	for (int i = 0; i < buffer; i++)
	{
		parsedString += timeStr[i];
	}

    return parsedString;
}