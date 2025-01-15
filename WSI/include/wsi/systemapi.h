#pragma once
#include <string>
#include <vector>
#include "common.h"

WSI_API enum ActivationStatus {
	UNLICENSED,         // The system is not activated; no valid license is installed.
	LICENSED,           // The system is fully activated and licensed.
	OOB_GRACE,          // Out-of-Box Grace: The system is within the initial grace period after installation but not yet activated.
	OOT_GRACE,          // Out-of-Tolerance Grace: Significant hardware changes detected; reactivation is required.
	NON_GENUINE_GRACE,  // Non-Genuine Grace: The system is flagged as non-genuine due to an invalid or counterfeit license.
	NOTIFICATION,       // Notification Mode: The system is functional but requires activation, with periodic reminders.
	EXTENDED_GRACE,     // Extended Grace Period: The system has additional time to activate, typically for volume licenses.
	UNAVAILABLE			// The system activation status is not available.
};

struct WSI_API ProcessorDetails {
	std::wstring vendor;
	int64_t architecture;
	int64_t core_count;
	int64_t thread_count;
};

struct WSI_API OperatingSystemDetails {
	std::wstring caption;
	std::wstring version;
	std::wstring build_number;
	std::wstring architecture;
	std::wstring language;
	ActivationStatus activation_status;
};

struct WSI_API DiskDetails {
	std::wstring model;
	std::wstring serial_number;
	std::wstring size;
	std::wstring media_type;
	std::wstring status;
};

int WSI_API queryProcessorDetails(ProcessorDetails& processor_details);
int WSI_API queryUserName(std::wstring& user_name);
int WSI_API queryComputerName(std::wstring& computer_name);
int WSI_API queryOperatingSystemDetails(OperatingSystemDetails& operating_system_details);
int WSI_API queryDiskDetails(std::vector<DiskDetails>& disk_details);