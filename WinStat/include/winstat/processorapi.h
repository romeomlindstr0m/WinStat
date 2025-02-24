#pragma once
#include <string>
#include <cstdint>
#include "winstat/common.h"

// Represents detailed information about the processor (CPU).
struct WINSTAT_API ProcessorDetails {
    std::wstring vendor;        // The vendor name of the processor (e.g., "Intel", "AMD").
    int64_t architecture;       // The processor architecture (e.g., 0 for x86, 9 for x64, 5 for ARM).
    int64_t core_count;         // The number of physical cores in the processor.
    int64_t thread_count;       // The number of logical threads supported by the processor.
    std::wstring serial_number;
};

// Queries detailed information about the system's processor.
int WINSTAT_API queryProcessorDetails(ProcessorDetails& processor_details);