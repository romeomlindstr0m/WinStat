#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <unordered_set>
#include "wsi/internal.h"

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
		return ERROR_INTERNAL_SID_INIT_FAILED;
	}

	BOOL query_token_membership = CheckTokenMembership(
		NULL,
		administrator_group,
		&is_member);

	if (query_token_membership == 0) {
		FreeSid(administrator_group);
		return ERROR_INTERNAL_QUERY_TOKEN_FAILED;
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
		L"[System Process]",
		L"Secure System",
		L"System",
		L"Registry",
		L"Memory Compression",
		L"smss.exe",
		L"csrss.exe",
		L"wininit.exe",
		L"lsass.exe",
		L"services.exe",
		L"svchost.exe",
		L"explorer.exe",
		L"winlogon.exe",
		L"taskmgr.exe",
		L"LsaIso.exe",
		L"MsMpEng.exe",
		L"TrustedInstaller.exe",
		L"WmiPrvSE"
	};

	if (restricted_pids.find(process_id) != restricted_pids.end()) { return true; }
	if (restricted_names.find(process_name) != restricted_names.end()) { return true; }

	return false;
}