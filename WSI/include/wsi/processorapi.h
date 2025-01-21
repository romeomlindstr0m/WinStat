#pragma once
#include <string>
#include <cstdint>
#include "wsi/common.h"

struct WSI_API ProcessorDetails {
	std::wstring vendor;
	int64_t architecture;
	int64_t core_count;
	int64_t thread_count;
};

int WSI_API queryProcessorDetails(ProcessorDetails& processor_details);