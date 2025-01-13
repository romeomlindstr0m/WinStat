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

// Warning codes are defined within a range of 10000-20000.
constexpr int WARNING_WMI_CLASS_NOT_SET = 10000; // WMI class not set.
constexpr int WARNING_WMI_PARTIAL_DATA = 10010; // A WMI query returned partial data, some properties may be missing.