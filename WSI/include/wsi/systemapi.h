#pragma once
#include <string>
#include <vector>
#include "common.h"

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