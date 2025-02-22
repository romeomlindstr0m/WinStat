#pragma once
#include "winstat/common.h"

// Represents the charging status of the system's battery.
WINSTAT_API enum ChargingStatus {
	CONNECTED = 0,		// The device is plugged in and charging.
	DISCONNECTED = 1,	// The device is running on battery power.
	UNKNOWN = 2			// The charging status cannot be determined.
};

// Provides battery information, including charge level and charging status.
struct WINSTAT_API BatteryStatus {
	int percentage;			// Battery level in percentage (0-100%).
	ChargingStatus status;	// Current charging status of the battery.
};

// Retrieves the current battery status.
int WINSTAT_API getBatteryStatus(BatteryStatus& battery_status);
// Shuts down or restarts the system (requires elevated privileges).
int WINSTAT_API shutdownSystem(bool restart = false);
// Logs out the current user and ends their session.
int WINSTAT_API logoffSystem();