#pragma once
#include <string>
#include <vector>
#include "winstat/common.h"

// Represents the activation status of the operating system.
WINSTAT_API enum ActivationStatus {
	UNLICENSED,         // The system is not activated; no valid license is installed.
	LICENSED,           // The system is fully activated and licensed.
	OOB_GRACE,          // Out-of-Box Grace: The system is within the initial grace period after installation but not yet activated.
	OOT_GRACE,          // Out-of-Tolerance Grace: Significant hardware changes detected; reactivation is required.
	NON_GENUINE_GRACE,  // Non-Genuine Grace: The system is flagged as non-genuine due to an invalid or counterfeit license.
	NOTIFICATION,       // Notification Mode: The system is functional but requires activation, with periodic reminders.
	EXTENDED_GRACE,     // Extended Grace Period: The system has additional time to activate, typically for volume licenses.
	UNAVAILABLE			// The system activation status is not available.
};

// Represents detailed information about the operating system.
struct WINSTAT_API OperatingSystemDetails {
	std::wstring caption;				// The name or caption of the operating system (e.g., "Windows 11 Pro").
	std::wstring version;				// The version of the operating system (e.g., "10.0.22621").
	std::wstring build_number;			// The build number of the operating system (e.g., "22621").
	std::wstring architecture;			// The system architecture (e.g., "x64", "ARM").
	std::wstring language;				// The default system language (e.g., "en-US").
	ActivationStatus activation_status; // The activation status of the operating system.
};

// Retrieves the username of the currently logged-in user.
int WINSTAT_API queryUserName(std::wstring& user_name);
// Retrieves the computer name of the system.
int WINSTAT_API queryComputerName(std::wstring& computer_name);
// Queries detailed information about the operating system.
int WINSTAT_API queryOperatingSystemDetails(OperatingSystemDetails& operating_system_details);