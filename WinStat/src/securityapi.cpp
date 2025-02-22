#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <processthreadsapi.h>
#include <wchar.h>
#include <string>
#include "winstat/securityapi.h"
#include "winstat/internal.h"

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

	PrivilegeManager privilege_manager;
	std::wstring privilege_name = L"SeSystemEnvironmentPrivilege";
	int privilege_adjust_res = privilege_manager.enablePrivilege(privilege_name);

	if (!IS_SUCCESS(privilege_adjust_res)) {
		return ERROR_SECURITY_PRIVILEGE_ADJUST_FAILED;
	}

	DWORD secure_boot_state = 0;
	DWORD secure_boot_query_res = GetFirmwareEnvironmentVariableExW(
		L"SecureBoot",
		L"{8be4df61-93ca-11d2-aa0d-00e098032b8c}",
		&secure_boot_state,
		sizeof(secure_boot_state),
		NULL);

	int privilege_revert_res = privilege_manager.revert();

	if (!IS_SUCCESS(privilege_revert_res)) {
		return ERROR_SECURITY_TOKEN_READJUST_FAILED;
	}

	if (secure_boot_query_res == 0) {
		return ERROR_SECURITY_SECURE_BOOT_QUERY_FAILED;
	}

	secure_boot_state == 1 ? state = true : state = false;
	return SUCCESS;
}