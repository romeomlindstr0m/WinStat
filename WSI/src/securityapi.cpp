#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <processthreadsapi.h>
#include <wchar.h>
#include <string>
#include "wsi/securityapi.h"
#include "wsi/internal.h"

int querySecureBootState(bool& state) {
	bool elevated_privileges = false;
	int privilege_res = isRunningAsAdmin(elevated_privileges);

	if (!IS_SUCCESS(privilege_res)) {
		return privilege_res;
	}

	if (!elevated_privileges) { return ERROR_SECURITY_ELEVATION_REQUIRED; }

	if (!isSystemUEFI()) {
		return ERROR_SECURITY_SYSTEM_NOT_UEFI;
	}

	HANDLE process_token = INVALID_HANDLE_VALUE;
	BOOL token_res = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &process_token);

	if (token_res == 0 && GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		return ERROR_SECURITY_PROCESS_TOKEN_FAILED;
	}

	LUID privilege_luid;
	BOOL privilege_query_res = LookupPrivilegeValueW(
		NULL,
		L"SeSystemEnvironmentPrivilege",
		&privilege_luid);

	if (privilege_query_res == 0) {
		CloseHandle(process_token);
		return ERROR_SECURITY_PRIVILEGE_LOOKUP_FAILED;
	}

	TOKEN_PRIVILEGES token_privileges;
	token_privileges.PrivilegeCount = 1;
	token_privileges.Privileges[0].Luid = privilege_luid;
	token_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	TOKEN_PRIVILEGES prev_token_privileges;
	DWORD prev_token_privileges_sz = sizeof(prev_token_privileges);

	BOOL token_adjust_res = AdjustTokenPrivileges(
		process_token,
		FALSE,
		&token_privileges,
		sizeof(prev_token_privileges),
		&prev_token_privileges,
		&prev_token_privileges_sz);

	if (token_adjust_res == 0) {
		CloseHandle(process_token);
		return ERROR_SECURITY_TOKEN_ADJUST_FAILED;
	}

	DWORD secure_boot_state = 0;
	DWORD secure_boot_query_res = GetFirmwareEnvironmentVariableExW(
		L"SecureBoot",
		L"{8be4df61-93ca-11d2-aa0d-00e098032b8c}",
		&secure_boot_state,
		sizeof(secure_boot_state),
		NULL);

	if (secure_boot_query_res == 0) {
		CloseHandle(process_token);
		return ERROR_SECURITY_SECURE_BOOT_QUERY_FAILED;
	}

	secure_boot_state == 1 ? state = true : state = false;

	BOOL token_adjust_reset_res = AdjustTokenPrivileges(
		process_token,
		FALSE,
		&prev_token_privileges,
		NULL, NULL, NULL);

	if (token_adjust_reset_res == 0 && GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		CloseHandle(process_token);
		return ERROR_SECURITY_TOKEN_READJUST_FAILED;
	}

	CloseHandle(process_token);
	return SUCCESS;
}