#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "wsi/common.h"

int WSI_API getProcessIdByName(std::wstring process_name, uint32_t& process_id, bool case_sensitive = true);
int WSI_API enumerateProcesses(std::unordered_map<uint32_t, std::wstring>& process_list);

int WSI_API terminateProcessById(uint32_t process_id);
int WSI_API terminateProcessByName(std::wstring process_name, bool case_sensitive = true);