#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "wsi/memoryapi.h"

int queryMemoryDetails(MemoryDetails& memory_details) {
	MEMORYSTATUSEX memory_status = { 0 };
	memory_status.dwLength = sizeof(MEMORYSTATUSEX);

	if (GlobalMemoryStatusEx(&memory_status) == FALSE) {
		return ERROR_MEMORY_STATUS_FAILED;
	}

	memory_details.load = static_cast<uint64_t>(memory_status.dwMemoryLoad);
	memory_details.total_physical = static_cast<uint64_t>(memory_status.ullTotalPhys);
	memory_details.available_physical = static_cast<uint64_t>(memory_status.ullAvailPhys);

	memory_details.total_page = static_cast<uint64_t>(memory_status.ullTotalPageFile);
	memory_details.available_page = static_cast<uint64_t>(memory_status.ullAvailPageFile);

	return SUCCESS;
}