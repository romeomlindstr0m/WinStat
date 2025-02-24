#pragma once
#include "winstat/common.h"

// Represents the type of time to retrieve.
WINSTAT_API enum Time {
	SYSTEM = 10,	// Retrieve the system (UTC) time.
	LOCAL = 20		// Retrieve the local time.
};

// Stores the current time.
struct WINSTAT_API CurrentTime {
	unsigned int year;			// The year (e.g., 2025).
	unsigned int month;			// The month (1-12).
	unsigned int week_day;		// The day of the week (0 = Sunday, 6 = Saturday).
	unsigned int day;			// The day of the month (1-31).
	unsigned int hours;			// The hour (0-23).
	unsigned int minutes;		// The minutes (0-59).
	unsigned int seconds;		// The seconds (0-59).
	unsigned int milliseconds;	// The milliseconds (0-999).
};

// Retrieves the current time based on the specified time type.
int WINSTAT_API queryCurrentTime(CurrentTime& current_time, Time time = Time::SYSTEM);