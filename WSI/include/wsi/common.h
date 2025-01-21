#pragma once

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
#ifdef WSI_EXPORT
#define WSI_API __declspec(dllexport)
#else
#define WSI_API __declspec(dllimport)
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
constexpr int ERROR_PROCESS_ID_NOT_VALID = 1170; // The process is not valid (the SYSTEM process cannot be terminated).
constexpr int ERROR_PROCESS_FULL_PATH_FAILED = 1180; // The process full path could not be retrieved.

// Warning codes are defined within a range of 10000-20000.
constexpr int WARNING_WMI_CLASS_NOT_SET = 10000; // WMI class not set.
constexpr int WARNING_WMI_PARTIAL_DATA = 10010; // A WMI query returned partial data, some properties may be missing.

constexpr int WARNING_PROCESS_NOT_FOUND = 10020; // A process with the matching name was not found.