#pragma once
#include <string>
#include <vector>
#include "wsi/common.h"

struct WSI_API GPUDetails {
	std::wstring name;
	std::wstring driver_version;
	int64_t total_memory;
	std::wstring status;
	int64_t current_refresh_rate;
	int64_t max_refresh_rate;
};

struct WSI_API MonitorDetails {
	std::wstring name;
	std::wstring manufacturer;
	int64_t height;
	int64_t width;
	std::wstring status;
};

int WSI_API queryGPUDetails(std::vector<GPUDetails>& gpu_details);
int WSI_API queryMonitorDetails(std::vector<MonitorDetails>& monitor_details);