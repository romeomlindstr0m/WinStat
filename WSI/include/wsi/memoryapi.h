#pragma once
#include <cstdint>
#include "wsi/common.h"

// Represents detailed information about the system's memory usage and capacity.
struct WSI_API MemoryDetails {
    uint64_t load;                  // Memory load as a percentage (0-100%).
    uint64_t total_physical;        // Total physical memory in bytes (RAM installed on the system).
    uint64_t available_physical;    // Amount of physical memory currently available in bytes.
    uint64_t total_page;            // Total amount of page memory (virtual memory) in bytes.
    uint64_t available_page;        // Amount of page memory currently available in bytes.
};

// Queries detailed information about the system's memory usage and capacity.
int WSI_API queryMemoryDetails(MemoryDetails& memory_details);