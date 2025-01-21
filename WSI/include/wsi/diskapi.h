#pragma once
#include <string>
#include <vector>
#include "wsi/common.h"

struct WSI_API DiskDetails {
	std::wstring model;
	std::wstring serial_number;
	std::wstring size;
	std::wstring media_type;
	std::wstring status;
};

int WSI_API queryDiskDetails(std::vector<DiskDetails>& disk_details);