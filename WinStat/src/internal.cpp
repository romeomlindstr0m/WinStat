#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <unordered_set>
#include <tlhelp32.h>
#include <algorithm>
#include <cwctype>
#include "winstat/internal.h"

int isRunningAsAdmin(bool& has_elevated_privileges) {
	BOOL is_member = FALSE;
	PSID administrator_group = NULL;

	SID_IDENTIFIER_AUTHORITY NT_authority = SECURITY_NT_AUTHORITY;

	BOOL SID_init = AllocateAndInitializeSid(
		&NT_authority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&administrator_group);

	if (SID_init == 0) {
		return ERROR_SID_INIT_FAILED;
	}

	BOOL query_token_membership = CheckTokenMembership(
		NULL,
		administrator_group,
		&is_member);

	if (query_token_membership == 0) {
		FreeSid(administrator_group);
		return ERROR_TOKEN_QUERY_FAILED;
	}

	has_elevated_privileges = is_member;

	FreeSid(administrator_group);
	return SUCCESS;
}

bool isSystemUEFI() {
	DWORD firmware_query_buffer = 0;
	DWORD firmware_query_res = GetFirmwareEnvironmentVariableExW(
		L"",
		L"{00000000-0000-0000-0000-000000000000}",
		&firmware_query_buffer,
		sizeof(firmware_query_buffer),
		NULL);

	if (GetLastError() == ERROR_INVALID_FUNCTION) {
		return false;
	}

	return true;
}

bool isProcessRestricted(uint32_t process_id, const std::wstring& process_name) {
	static const std::unordered_set<uint32_t> restricted_pids = {
		0, 4, 8, 12, 18
	};

	static const std::unordered_set<std::wstring> restricted_names = {
		L"[system process]",
		L"secure system",
		L"system",
		L"registry",
		L"memory compression",
		L"smss.exe",
		L"csrss.exe",
		L"wininit.exe",
		L"lsass.exe",
		L"services.exe",
		L"svchost.exe",
		L"explorer.exe",
		L"winlogon.exe",
		L"taskmgr.exe",
		L"lsaiso.exe",
		L"msmpeng.exe",
		L"trustedinstaller.exe",
		L"wmiprvse.exe"
	};

	std::wstring process_name_buffer = std::wstring(process_name);
	std::transform(process_name_buffer.begin(), process_name_buffer.end(), process_name_buffer.begin(), std::tolower);

	if (restricted_pids.find(process_id) != restricted_pids.end()) { return true; }
	if (restricted_names.find(process_name_buffer) != restricted_names.end()) { return true; }

	return false;
}

int enumerateProcesses(std::vector<ProcessInfo>& process_list, bool enumerate_restricted) {
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

		if (!isProcessRestricted(process_entry.th32ProcessID, process_name) || enumerate_restricted) {
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

class PrivilegeManager::PrivilegeManagerImpl {
public:
	PrivilegeManagerImpl() {
		process_token_ = INVALID_HANDLE_VALUE;
		process_token_active_ = false;
	}

	~PrivilegeManagerImpl() {
		if (process_token_active_) {
			CloseHandle(process_token_);
			process_token_ = INVALID_HANDLE_VALUE;
		}
	}

	int enablePrivilege(const std::wstring& privilege_name) {
		bool elevated_privileges = false;
		int privilege_res = isRunningAsAdmin(elevated_privileges);

		if (!IS_SUCCESS(privilege_res)) {
			return privilege_res;
		}

		if (!elevated_privileges) {
			return ERROR_SECURITY_ELEVATION_REQUIRED;
		}

		BOOL token_res = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &process_token_);

		if (token_res == 0 && GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
			return ERROR_TOKEN_ACCESS_FAILED;
		}

		process_token_active_ = true;

		LUID privilege_luid;
		BOOL privilege_query_res = LookupPrivilegeValueW(
			NULL,
			privilege_name.c_str(),
			&privilege_luid);

		if (privilege_query_res == 0) {
			return ERROR_PRIVILEGE_LOOKUP_FAILED;
		}

		TOKEN_PRIVILEGES token_privileges;
		token_privileges.PrivilegeCount = 1;
		token_privileges.Privileges[0].Luid = privilege_luid;
		token_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		DWORD prev_token_privileges_sz = sizeof(prev_token_privileges_);

		BOOL token_adjust_res = AdjustTokenPrivileges(
			process_token_,
			FALSE,
			&token_privileges,
			sizeof(prev_token_privileges_),
			&prev_token_privileges_,
			&prev_token_privileges_sz);

		if (token_adjust_res == 0) {
			return ERROR_SECURITY_PRIVILEGE_ADJUST_FAILED;
		}

		return SUCCESS;
	}

	int revert() {
		BOOL token_adjust_reset_res = AdjustTokenPrivileges(
			process_token_,
			FALSE,
			&prev_token_privileges_,
			NULL, NULL, NULL);

		if (token_adjust_reset_res == 0 && GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
			return ERROR_TOKEN_READJUST_FAILED;
		}

		return SUCCESS;
	}

private:
	TOKEN_PRIVILEGES prev_token_privileges_;
	HANDLE process_token_;
	bool process_token_active_;
};

PrivilegeManager::PrivilegeManager() {
	impl_ = new PrivilegeManagerImpl;
}

PrivilegeManager::~PrivilegeManager() {
	delete impl_;
}

int PrivilegeManager::enablePrivilege(const std::wstring& privilege_name) {
	return impl_->enablePrivilege(privilege_name);
}

int PrivilegeManager::revert() {
	return impl_->revert();
}