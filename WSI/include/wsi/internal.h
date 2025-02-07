#pragma once
#include <cstdint>
#include <string>
#include "wsi/common.h"

// Checks if the current process is running with administrator privileges, this function is for internal use only and should not be called.
int isRunningAsAdmin(bool& has_elevated_privileges);
// Determines whether the system is running in UEFI or Legacy BIOS mode, this function is for internal use only and should not be called.
bool isSystemUEFI();

bool isProcessRestricted(uint32_t process_id, const std::wstring& process_name);