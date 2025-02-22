#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "winstat/powerapi.h"
#include "winstat/internal.h"

int getBatteryStatus(BatteryStatus& battery_status) {
	SYSTEM_POWER_STATUS system_power;
	if (GetSystemPowerStatus(&system_power) == 0) {
		return ERROR_BATTERY_STATUS_FAILED;
	}

	switch (system_power.ACLineStatus) {
	case 0: battery_status.status = ChargingStatus::DISCONNECTED; break;
	case 1: battery_status.status = ChargingStatus::CONNECTED; break;
	default: battery_status.status = ChargingStatus::UNKNOWN; break;
	}

	if (system_power.BatteryLifePercent == 255) {
		battery_status.percentage = 0;
		return WARNING_BATTERY_PERCENTAGE_UNKNOWN;
	}

	battery_status.percentage = static_cast<int>(system_power.BatteryLifePercent);
	return SUCCESS;
}

int shutdownSystem(bool restart) {
	PrivilegeManager privilege_manager;
	std::wstring privilege_name = L"SeShutdownPrivilege";
	int privilege_adjust_res = privilege_manager.enablePrivilege(privilege_name);

	if (!IS_SUCCESS(privilege_adjust_res)) {
		return privilege_adjust_res;
	}

	#pragma warning ( push )
	#pragma warning ( disable : 28159 )
	BOOL shutdown_res = InitiateSystemShutdownExW(
		NULL, NULL, 0,
		FALSE,
		restart ? TRUE : FALSE,
		SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_OTHER | SHTDN_REASON_FLAG_PLANNED);
	#pragma warning ( pop )

	if (shutdown_res == 0) {
		if (GetLastError() == ERROR_SHUTDOWN_IN_PROGRESS) { return ERROR_SYSTEM_SHUTDOWN_IN_PROGRESS; }
		else {
			return ERROR_SYSTEM_SHUTDOWN_FAILED;
		}
	}

	int privilege_reset_res = privilege_manager.revert();

	if (!IS_SUCCESS(privilege_reset_res)) {
		return privilege_reset_res;
	}

	return SUCCESS;
}