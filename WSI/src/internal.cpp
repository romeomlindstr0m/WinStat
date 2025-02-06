#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
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