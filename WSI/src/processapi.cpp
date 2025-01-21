#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <tlhelp32.h>
#include <processthreadsapi.h>
#include "wsi/processapi.h"

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

int enumerateProcesses(std::unordered_map<uint32_t, std::wstring>& process_list) {
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

	process_list.insert({ static_cast<uint32_t>(process_entry.th32ProcessID), process_entry.szExeFile });

	do {
		process_list.insert({ static_cast<uint32_t>(process_entry.th32ProcessID), process_entry.szExeFile });
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