#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT 0x0600
#include <Windows.h>
#include <tlhelp32.h>
#include <processthreadsapi.h>
#include "wsi/processapi.h"
#include "wsi/internal.h"

namespace {
	bool isValidProcessId(uint32_t process_id) {
		// The SYSTEM process cannot be terminated.
		if (process_id == 4) {
			return false;
		}

		HANDLE validation_handle = OpenProcess(
			PROCESS_QUERY_INFORMATION,
			FALSE,
			static_cast<DWORD>(process_id));

		if (validation_handle == NULL) {
			return false;
		}

		CloseHandle(validation_handle);
		return true;
	}
}

int getProcessIdByName(std::wstring process_name, uint32_t& process_id, bool case_sensitive) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (snapshot == INVALID_HANDLE_VALUE) {
		return ERROR_PROCESS_SNAPSHOT_FAILED;
	}

	PROCESSENTRY32W process_entry = { 0 };
	process_entry.dwSize = sizeof(PROCESSENTRY32W);

	if (Process32FirstW(snapshot, &process_entry) == FALSE) {
		CloseHandle(snapshot);
		return ERROR_PROCESS_FIRST_MISSING;
	}

	const wchar_t* process_name_buffer = process_name.c_str();

	do {
		if (case_sensitive) {
			if (wcscmp(process_entry.szExeFile, process_name_buffer) == 0) {
				process_id = process_entry.th32ProcessID;
				CloseHandle(snapshot);
				return SUCCESS;
			}
		}
		else {
			if (_wcsicmp(process_entry.szExeFile, process_name_buffer) == 0) {
				process_id = process_entry.th32ProcessID;
				CloseHandle(snapshot);
				return SUCCESS;
			}
		}

	} while (Process32NextW(snapshot, &process_entry) != FALSE);

	CloseHandle(snapshot);
	return WARNING_PROCESS_NOT_FOUND;
}

int enumerateProcesses(std::vector<ProcessInfo>& process_list) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (snapshot == INVALID_HANDLE_VALUE) {
		return ERROR_PROCESS_SNAPSHOT_FAILED;
	}

	PROCESSENTRY32W process_entry;
	process_entry.dwSize = sizeof(PROCESSENTRY32W);

	if (Process32FirstW(snapshot, &process_entry) == FALSE) {
		CloseHandle(snapshot);
		return ERROR_PROCESS_FIRST_MISSING;
	}

	do {
		std::wstring process_name = std::wstring(process_entry.szExeFile);

		if (!isProcessRestricted(process_entry.th32ProcessID, process_name)) {
			ProcessInfo process_info;
			process_info.id = static_cast<uint32_t>(process_entry.th32ProcessID);
			process_info.parent_id = static_cast<uint32_t>(process_entry.th32ParentProcessID);
			process_info.name = std::wstring(process_entry.szExeFile);

			HANDLE process_handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_entry.th32ProcessID);

			if (process_handle != NULL) {
				DWORD process_path_buffer_sz = MAX_PATH;
				WCHAR process_path_buffer[MAX_PATH];

				if (QueryFullProcessImageNameW(process_handle, 0, process_path_buffer, &process_path_buffer_sz) != 0) {
					process_info.executable_path = std::wstring(process_path_buffer);
				}
				else {
					process_info.executable_path = L"";
				}

				CloseHandle(process_handle);
			}
			else {
				process_info.executable_path = L"";
			}

			process_list.push_back(process_info);
		}

	} while (Process32NextW(snapshot, &process_entry) != FALSE);
	
	CloseHandle(snapshot);
	return SUCCESS;
}

int terminateProcessById(uint32_t process_id) {
	if (!isValidProcessId(process_id)) {
		return ERROR_PROCESS_ID_NOT_VALID;
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
	if (!isValidProcessId(process_id)) {
		return process_id;
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
	if (!isValidProcessId(process_id)) {
		return ERROR_PROCESS_ID_NOT_VALID;
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