#pragma once
#include <cstdint>
#include "wsi/common.h"

struct WSI_API MemoryDetails {
	uint64_t load;				// Memory load represented in a range of 0-100%.
	uint64_t total_physical;	// Total amount of physical memory in bytes.
	uint64_t available_physical;// Amount of physical memory available.
	uint64_t total_page;		// Total amount of page memory.
	uint64_t available_page;	// Amount of available page memory.
};

int WSI_API queryMemoryDetails(MemoryDetails& memory_details);