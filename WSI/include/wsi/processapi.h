#pragma once
#include <cstdint>
#include <string>
#include "wsi/common.h"

int WSI_API GetProcessIdByName(std::wstring process_name, uint32_t& process_id, bool case_sensitive = true);

int WSI_API TerminateProcessById(uint32_t process_id);
int WSI_API TerminateProcessByName(std::wstring process_name, bool case_sensitive = true);