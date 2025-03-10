#pragma once
#include <string>
#include <vector>
#include "winstat/common.h"

// Represents details information about a disk such as its model serial number and media type.
struct WINSTAT_API DiskDetails {
	std::wstring model;
	std::wstring serial_number;
	std::wstring size;
	std::wstring media_type;
	std::wstring status;
};

// Queries the details of all available disks on the system and populates the provided vector.
int WINSTAT_API queryDiskDetails(std::vector<DiskDetails>& disk_details);