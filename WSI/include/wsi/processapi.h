#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "wsi/common.h"

WSI_API enum Priority {
	BACKGROUND_BEGIN = 0,
	BACKGROUND_END = 1,
	REALTIME = 2,
	HIGH = 3,
	ABOVE_NORMAL = 4,
	NORMAL = 5,
	BELOW_NORMAL = 6,
	IDLE = 7,
	UNDEFINED = 8
};

int WSI_API getProcessIdByName(std::wstring process_name, uint32_t& process_id, bool case_sensitive = true);
int WSI_API enumerateProcesses(std::unordered_map<uint32_t, std::wstring>& process_list);
int WSI_API getExecutablePath(uint32_t process_id, std::wstring& executable_path);

int WSI_API terminateProcessById(uint32_t process_id);
int WSI_API terminateProcessByName(std::wstring process_name, bool case_sensitive = true);

int WSI_API getProcessPriority(uint32_t process_id, Priority& process_priority);
int WSI_API setProcessPriority(uint32_t process_id, Priority& process_priority);