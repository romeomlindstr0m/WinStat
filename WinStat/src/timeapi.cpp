#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "winstat/timeapi.h"

int queryCurrentTime(CurrentTime& current_time, Time time) {
	if (time != Time::LOCAL && time != Time::SYSTEM) {
		return ERROR_TIME_VALUE_INVALID;
	}

	SYSTEMTIME system_time{};

	if (time == Time::LOCAL) {
		GetLocalTime(&system_time);

		current_time.year = static_cast<unsigned int>(system_time.wYear);
		current_time.month = static_cast<unsigned int>(system_time.wMonth);
		current_time.week_day = static_cast<unsigned int>(system_time.wDayOfWeek);
		current_time.day = static_cast<unsigned int>(system_time.wDay);
		current_time.hours = static_cast<unsigned int>(system_time.wMinute);
		current_time.minutes = static_cast<unsigned int>(system_time.wMinute);
		current_time.seconds = static_cast<unsigned int>(system_time.wSecond);
		current_time.milliseconds = static_cast<unsigned int>(system_time.wMilliseconds);
	}
	else {
		GetSystemTime(&system_time);

		current_time.year = static_cast<unsigned int>(system_time.wYear);
		current_time.month = static_cast<unsigned int>(system_time.wMonth);
		current_time.week_day = static_cast<unsigned int>(system_time.wDayOfWeek);
		current_time.day = static_cast<unsigned int>(system_time.wDay);
		current_time.hours = static_cast<unsigned int>(system_time.wMinute);
		current_time.minutes = static_cast<unsigned int>(system_time.wMinute);
		current_time.seconds = static_cast<unsigned int>(system_time.wSecond);
		current_time.milliseconds = static_cast<unsigned int>(system_time.wMilliseconds);
	}

	return SUCCESS;
}