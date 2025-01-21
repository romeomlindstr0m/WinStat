#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "wsi/common.h"

// Represents the priority level of a process.
WSI_API enum Priority {
    BACKGROUND_BEGIN = 0, // Set the process to background mode (low I/O and memory priority).
    BACKGROUND_END = 1,   // Restore the process from background mode to normal priority.
    REALTIME = 2,         // Highest priority level for time-critical tasks.
    HIGH = 3,             // High priority for important tasks.
    ABOVE_NORMAL = 4,     // Priority slightly above normal.
    NORMAL = 5,           // Default priority level.
    BELOW_NORMAL = 6,     // Priority slightly below normal.
    IDLE = 7,             // Lowest priority, suitable for background tasks.
    UNDEFINED = 8         // Placeholder for undefined priorities.
};

// Retrieves the process ID of a process by its name.
int WSI_API getProcessIdByName(std::wstring process_name, uint32_t& process_id, bool case_sensitive = true);
// Enumerates all running processes on the system.
int WSI_API enumerateProcesses(std::unordered_map<uint32_t, std::wstring>& process_list);
// Retrieves the full executable path of a process by its ID.
int WSI_API getExecutablePath(uint32_t process_id, std::wstring& executable_path);
// Terminates a process by its ID.
int WSI_API terminateProcessById(uint32_t process_id);
// Terminates a process by its name.
int WSI_API terminateProcessByName(std::wstring process_name, bool case_sensitive = true);
// Retrieves the priority level of a process by its ID.
int WSI_API getProcessPriority(uint32_t process_id, Priority& process_priority);
// Sets the priority level of a process by its ID.
int WSI_API setProcessPriority(uint32_t process_id, Priority& process_priority);