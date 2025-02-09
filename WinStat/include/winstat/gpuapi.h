#pragma once
#include <string>
#include <vector>
#include "winstat/common.h"

// Represents detailed information about a GPU (Graphics Processing Unit).
struct WINSTAT_API GPUDetails {
	std::wstring name;				// The name of the GPU (e.g., "NVIDIA GeForce RTX 3080").
	std::wstring driver_version;	// The version of the GPU driver (e.g., "531.18").
	int64_t total_memory;			// The total memory of the GPU in bytes.
	std::wstring status;			// The operational status of the GPU (e.g., "Active", "Disabled").
	int64_t current_refresh_rate;	// The current refresh rate of the GPU in Hz.
	int64_t max_refresh_rate;		// The maximum supported refresh rate of the GPU in Hz.
};

// Represents detailed information about a monitor.
struct WINSTAT_API MonitorDetails {
	std::wstring name;				// The name of the monitor (e.g., "Dell U2723QE").
	std::wstring manufacturer;		// The manufacturer of the monitor (e.g., "Dell").
	int64_t height;					// The height of the monitor in pixels.
	int64_t width;					// The width of the monitor in pixels.
	std::wstring status;			// The operational status of the monitor (e.g., "Connected", "Disconnected").
};

// Queries the details of all available GPUs on the system and populates the provided vector.
int WINSTAT_API queryGPUDetails(std::vector<GPUDetails>& gpu_details);
// Queries the details of all available monitors on the system and populates the provided vector.
int WINSTAT_API queryMonitorDetails(std::vector<MonitorDetails>& monitor_details);