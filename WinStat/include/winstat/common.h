#pragma once
#define _WIN32_WINNT _WIN32_WINNT_WIN10

/*
common.h

Description:
	This header file defines constants for success, error, and warning codes
	used throughout the project. It also provides a preprocessor directive
	for exporting/importing library components.

Usage:
	Include this header where access to status codes are required.
*/

// Define macro to check if a function or method succeeded without errors
#define IS_SUCCESS(code) ((code) >= 0 && (code) <= 999)

// Define preprocessor directive to export/import library components.
#ifdef WINSTAT_EXPORT
#define WINSTAT_API __declspec(dllexport)
#else
#define WINSTAT_API __declspec(dllimport)
#endif

// Success codes are defined within a range of 0-999.
constexpr int SUCCESS = 0;

// Error codes are defined within a range of 1000-9999.
constexpr int ERROR_WMI_INIT_FAILED = 1000; // WMI initialization failed.
constexpr int ERROR_WMI_SEC_FAILED = 1010; // Failed to set WMI security.
constexpr int ERROR_WMI_LOCATOR_FAILED = 1020; // Failed to create WMI locator.

constexpr int ERROR_WMI_CONN_FAILED = 1030; // Failed to connect to default WMI namespace.
constexpr int ERROR_WMI_PROXY_FAILED = 1040; // Failed to set COM proxy blanket.
constexpr int ERROR_WMI_QUERY_FAILED = 1050; // WMI query failed.

constexpr int ERROR_WMI_PROPERTY_NOT_FOUND = 1060; // WMI property not found.
constexpr int ERROR_WMI_PROPERTY_TYPE_NOT_SUPPORTED = 1070; // WMI property type not supported.
constexpr int ERROR_WMI_NO_MORE_OBJECTS = 1080; // No more WMI class objects in enumerator.

constexpr int ERROR_QUERY_USER_NAME_FAILED = 1090; // User name query failed.
constexpr int ERROR_QUERY_COMPUTER_NAME_FAILED = 1100; // Computer name query failed.
constexpr int ERROR_UNEXPECTED_VARIANT_TYPE = 1110; // A WMI query returned a data type which was not expected

constexpr int ERROR_MEMORY_STATUS_FAILED = 1120; // Something went wrong while querying memory information.
constexpr int ERROR_PROCESS_SNAPSHOT_FAILED = 1130; // The system failed to create an snapshot of all running processes.
constexpr int ERROR_PROCESS_FIRST_MISSING = 1140; // The first process entry is missing, this may indicate that the process list is empty.

constexpr int ERROR_PROCESS_HANDLE_FAILED = 1150; // The system failed to create an handle to the specified process, this is usually because the calling application has insufficient permissions.
constexpr int ERROR_PROCESS_TERMINATION_FAILED = 1160; // The system failed to terminate the specified process.
constexpr int ERROR_PROCESS_RESTRICTED = 1170; // Process is restricted and cannot be enumerated or modified.

constexpr int ERROR_PROCESS_FULL_PATH_FAILED = 1180; // The process full path could not be retrieved.
constexpr int ERROR_PROCESS_PRIORITY_TYPE_UNKNOWN = 1190; // An unexpected process priority type was encountered.
constexpr int ERROR_PROCESS_PRIORITY_CANNOT_BE_UNDEFINED = 1200; // The process priority cannot be set to UNDEFINED.

constexpr int ERROR_PROCESS_PRIORITY_SET_FAILED = 1210; // The system failed to set the given priority.
constexpr int ERROR_SID_INIT_FAILED = 1220; // Failed to initialize the SID for the Administrators group.
constexpr int ERROR_TOKEN_QUERY_FAILED = 1230; // Failed to query token membership for the Administrators group.

constexpr int ERROR_TOKEN_ACCESS_FAILED = 1240; // Failed to open the current process token.
constexpr int ERROR_PRIVILEGE_LOOKUP_FAILED = 1250; // Failed to find the specified system privilege.
constexpr int ERROR_TOKEN_READJUST_FAILED = 1270; // Failed to restore the current process token to its original state.

constexpr int ERROR_SECURITY_ELEVATION_REQUIRED = 1280; // The operation requires elevated privileges (admin rights).
constexpr int ERROR_SECURITY_PROCESS_TOKEN_FAILED = 1290; // Failed to open or retrieve the process token.
constexpr int ERROR_SECURITY_PRIVILEGE_LOOKUP_FAILED = 1300; // Failed to find the required system privilege.

constexpr int ERROR_SECURITY_TOKEN_ADJUST_FAILED = 1310; // Failed to adjust required privileges in the process token.
constexpr int ERROR_SECURITY_SECURE_BOOT_QUERY_FAILED = 1320; // Secure Boot state could not be retrieved from the firmware.
constexpr int ERROR_SECURITY_TOKEN_READJUST_FAILED = 1330; // Failed to restore the process token to its original state after privilege adjustment.

constexpr int ERROR_SECURITY_SYSTEM_NOT_UEFI = 1340; // The system is not using UEFI firmware (likely running in Legacy BIOS mode).
constexpr int ERROR_SECURITY_PRIVILEGE_ADJUST_FAILED = 1350; // Failed to adjust the specified privilege in the current process token.
constexpr int ERROR_BATTERY_STATUS_FAILED = 1370; // Failed to retrieve battery status.

constexpr int ERROR_SYSTEM_SHUTDOWN_FAILED = 1380; // Failed to initiate system shutdown.
constexpr int ERROR_SYSTEM_SHUTDOWN_IN_PROGRESS = 1390; // A system shutdown has already been initiated.
constexpr int ERROR_USER_SESSION_LOGOFF_FAILED = 1400;

constexpr int ERROR_TIME_VALUE_INVALID = 1410; // The time parameter was set to an unsupported value.
constexpr int ERROR_FILE_NONEXISTENT = 1420;
constexpr int ERROR_FILE_GENERIC_ERROR = 1430;

constexpr int ERROR_FILE_WRITE_FAILED = 1440;
constexpr int ERROR_FILE_READ_FAILED = 1450;
constexpr int ERROR_FILE_READONLY = 1460;

constexpr int ERROR_FILE_SIZE_READ_FAILED = 1450;
constexpr int ERROR_UTF8_BUFFER_FAILED = 1460;
constexpr int ERROR_INVALID_CHARACTER_ENCOUNTERED = 1470;

constexpr int ERROR_CONVERSION_FAILED = 1480;
constexpr int ERROR_UTF16_BUFFER_FAILED = 1490;

// Warning codes are defined within a range of 10000-19999.
constexpr int WARNING_WMI_CLASS_NOT_SET = 10000; // WMI class not set.
constexpr int WARNING_WMI_PARTIAL_DATA = 10010; // A WMI query returned partial data, some properties may be missing.
constexpr int WARNING_PROCESS_NOT_FOUND = 10020; // A process with the matching name was not found.

constexpr int WARNING_BATTERY_PERCENTAGE_UNKNOWN = 10030; // Battery percentage could not be determined.

// Informational codes are defined within a range of 20000-29999
constexpr int INFO_FEATURE_NOT_IMPLEMENTED = 20000;