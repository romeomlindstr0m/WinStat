#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <tlhelp32.h>
#include <processthreadsapi.h>
#include <algorithm>
#include <cwctype>
#include "wsi/processapi.h"
#include "wsi/internal.h"

int getProcessIdByName(std::wstring process_name, uint32_t& process_id, bool case_sensitive) {
	std::vector<ProcessInfo> process_list;
	int enumeration_res = enumerateProcesses(process_list, true);

	if (!IS_SUCCESS(enumeration_res)) {
		return enumeration_res;
	}

	if (case_sensitive) {
		for (ProcessInfo& process : process_list) {
			if (process_name == process.name) {
				if (isProcessRestricted(process.id, process.name)) {
					return ERROR_PROCESS_RESTRICTED;
				}
				process_id = process.id;
				return SUCCESS;
			}
		}
	}
	else {
		for (ProcessInfo& process : process_list) {
			std::transform(process.name.begin(), process.name.end(), process.name.begin(), std::tolower);
			if (process.name == process_name) {
				if (isProcessRestricted(process.id, process.name)) {
					return ERROR_PROCESS_RESTRICTED;
				}
				process_id = process.id;
				return SUCCESS;
			}
		}
	}

	return WARNING_PROCESS_NOT_FOUND;
}

int WSI_API enumerateProcesses(std::vector<ProcessInfo>& process_list) {
	int enumeration_res = enumerateProcesses(process_list, false);

	if (!IS_SUCCESS(enumeration_res)) {
		return enumeration_res;
	}

	return SUCCESS;
}

int terminateProcessById(uint32_t process_id) {
	std::vector<ProcessInfo> process_list;
	int enumeration_res = enumerateProcesses(process_list, true);

	if (!IS_SUCCESS(enumeration_res)) {
		return enumeration_res;
	}

	for (ProcessInfo& process : process_list) {
		if (process.id == process_id) {
			if (isProcessRestricted(process.id, process.name)) {
				return ERROR_PROCESS_RESTRICTED;
			}
			else {
				break;
			}
		}
	}

	HANDLE process_handle = OpenProcess(PROCESS_TERMINATE, FALSE, static_cast<DWORD>(process_id));

	if (process_handle == NULL) {
		return ERROR_PROCESS_HANDLE_FAILED;
	}

	if (TerminateProcess(process_handle, 0) == 0) {
		CloseHandle(process_handle);
		return ERROR_PROCESS_TERMINATION_FAILED;
	}

	CloseHandle(process_handle);
	return SUCCESS;
}

int terminateProcessByName(std::wstring process_name, bool case_sensitive) {
	uint32_t target_process_id = 0;
	int res = SUCCESS;

	res = getProcessIdByName(process_name, target_process_id, case_sensitive);

	if (!IS_SUCCESS(res)) {
		return res;
	}

	return terminateProcessById(target_process_id);
}

int getProcessPriority(uint32_t process_id, Priority& process_priority) {
	std::vector<ProcessInfo> process_list;
	int enumeration_res = enumerateProcesses(process_list, true);

	if (!IS_SUCCESS(enumeration_res)) {
		return enumeration_res;
	}

	for (ProcessInfo& process : process_list) {
		if (process.id == process_id) {
			if (isProcessRestricted(process.id, process.name)) {
				return ERROR_PROCESS_RESTRICTED;
			}
			else {
				break;
			}
		}
	}

	HANDLE process_handle = OpenProcess(
		PROCESS_QUERY_LIMITED_INFORMATION,
		FALSE,
		static_cast<DWORD>(process_id));

	if (process_handle == NULL) {
		return ERROR_PROCESS_HANDLE_FAILED;
	}

	DWORD priority_value = GetPriorityClass(process_handle);
	CloseHandle(process_handle);

	switch (priority_value) {
	case ABOVE_NORMAL_PRIORITY_CLASS:
		process_priority = Priority::ABOVE_NORMAL;
		break;
	case BELOW_NORMAL_PRIORITY_CLASS:
		process_priority = Priority::BELOW_NORMAL;
		break;
	case HIGH_PRIORITY_CLASS:
		process_priority = Priority::HIGH;
		break;
	case IDLE_PRIORITY_CLASS:
		process_priority = Priority::IDLE;
		break;
	case NORMAL_PRIORITY_CLASS:
		process_priority = Priority::NORMAL;
		break;
	case REALTIME_PRIORITY_CLASS:
		process_priority = Priority::REALTIME;
		break;
	default:
		return ERROR_PROCESS_PRIORITY_TYPE_UNKNOWN;
		break;
	}

	return SUCCESS;
}

int setProcessPriority(uint32_t process_id, Priority& process_priority) {
	std::vector<ProcessInfo> process_list;
	int enumeration_res = enumerateProcesses(process_list, true);

	if (!IS_SUCCESS(enumeration_res)) {
		return enumeration_res;
	}

	for (ProcessInfo& process : process_list) {
		if (process.id == process_id) {
			if (isProcessRestricted(process.id, process.name)) {
				return ERROR_PROCESS_RESTRICTED;
			}
			else {
				break;
			}
		}
	}

	if (process_priority == Priority::UNDEFINED) {
		return ERROR_PROCESS_PRIORITY_CANNOT_BE_UNDEFINED;
	}

	HANDLE process_handle = OpenProcess(
		PROCESS_SET_INFORMATION,
		FALSE,
		process_id);

	if (process_handle == NULL) {
		return ERROR_PROCESS_HANDLE_FAILED;
	}

	DWORD process_priority_buffer = NORMAL_PRIORITY_CLASS;

	switch (process_priority) {
	case Priority::BACKGROUND_BEGIN:
		process_priority_buffer = PROCESS_MODE_BACKGROUND_BEGIN;
		break;
	case Priority::BACKGROUND_END:
		process_priority_buffer = PROCESS_MODE_BACKGROUND_END;
		break;
	case Priority::REALTIME:
		process_priority_buffer = REALTIME_PRIORITY_CLASS;
		break;
	case Priority::HIGH:
		process_priority_buffer = HIGH_PRIORITY_CLASS;
		break;
	case Priority::ABOVE_NORMAL:
		process_priority_buffer = ABOVE_NORMAL_PRIORITY_CLASS;
		break;
	case Priority::NORMAL:
		process_priority_buffer = NORMAL_PRIORITY_CLASS;
		break;
	case Priority::BELOW_NORMAL:
		process_priority_buffer = BELOW_NORMAL_PRIORITY_CLASS;
		break;
	case Priority::IDLE:
		process_priority_buffer = IDLE_PRIORITY_CLASS;
		break;
	}

	if (SetPriorityClass(process_handle, process_priority_buffer) == 0) {
		CloseHandle(process_handle);
		return ERROR_PROCESS_PRIORITY_SET_FAILED;
	}

	CloseHandle(process_handle);
	return SUCCESS;
}