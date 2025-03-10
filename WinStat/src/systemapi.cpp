#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <lmcons.h>
#include "winstat/systemapi.h"
#include "winstat/wmiapi.h"

int queryUserName(std::wstring& user_name) {
	wchar_t user_name_buffer[UNLEN + 1];
	DWORD character_count_buffer = UNLEN + 1;

	if (GetUserNameW(user_name_buffer, &character_count_buffer) == FALSE) {
		return ERROR_QUERY_USER_NAME_FAILED;
	}

	user_name = std::wstring(user_name_buffer);

	return SUCCESS;
}

int queryComputerName(std::wstring& computer_name) {
	wchar_t computer_name_buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD character_count_buffer = MAX_COMPUTERNAME_LENGTH + 1;

	if (GetComputerNameW(computer_name_buffer, &character_count_buffer) == FALSE) {
		return ERROR_QUERY_COMPUTER_NAME_FAILED;
	}

	computer_name = std::wstring(computer_name_buffer);

	return SUCCESS;
}

int WINSTAT_API queryOperatingSystemDetails(OperatingSystemDetails& operating_system_details) {
	std::variant<std::wstring, int64_t, uint64_t, bool> stored_property;
	WMIQueryManager wmi_query_manager;
	int wmi_res = SUCCESS;

	bool partial_data = false;

	wmi_res = wmi_query_manager.setWMIClass(L"SELECT Caption, Version, BuildNumber, OSArchitecture, Locale FROM Win32_OperatingSystem");

	if (wmi_res != SUCCESS) {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"Caption");

	if (wmi_res == SUCCESS) {
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<std::wstring>(stored_property)) {
			operating_system_details.caption = std::get<std::wstring>(stored_property);
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		operating_system_details.caption = L"";
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"Caption");

	if (wmi_res == SUCCESS) {
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<std::wstring>(stored_property)) {
			std::wstring caption_buffer = std::get<std::wstring>(stored_property);

			for (int i = caption_buffer.length() - 1; i >= 0; i--) {
				if (caption_buffer[i] == L' ') {
					caption_buffer.erase(i);
				}
				else {
					break;
				}
			}

			operating_system_details.caption = caption_buffer;
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		operating_system_details.caption = L"";
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"Version");

	if (wmi_res == SUCCESS) {
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<std::wstring>(stored_property)) {
			operating_system_details.version = std::get<std::wstring>(stored_property);
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		operating_system_details.version = L"";
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"BuildNumber");

	if (wmi_res == SUCCESS) {
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<std::wstring>(stored_property)) {
			operating_system_details.build_number = std::get<std::wstring>(stored_property);
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		operating_system_details.build_number = L"";
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"OSArchitecture");

	if (wmi_res == SUCCESS) {
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<std::wstring>(stored_property)) {
			operating_system_details.architecture = std::get<std::wstring>(stored_property);
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		operating_system_details.architecture = L"";
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"Locale");

	if (wmi_res == SUCCESS) {
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<std::wstring>(stored_property)) {
			operating_system_details.language = std::get<std::wstring>(stored_property);
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		operating_system_details.language = L"";
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.setWMIClass(L"SELECT ApplicationID, PartialProductKey, LicenseStatus, Description FROM SoftwareLicensingProduct WHERE PartialProductKey IS NOT NULL AND Description LIKE 'Windows%Operating%System%'");

	if (wmi_res != SUCCESS) {
		return wmi_res;
	}

	bool os_license = false;

	do {
		wmi_res = wmi_query_manager.queryWMIProperty(L"ApplicationID");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				if (std::get<std::wstring>(stored_property) != L"55c92734-d682-4d71-983e-d6ec3f16059f") {
					continue;
				}
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			operating_system_details.activation_status = ActivationStatus::UNAVAILABLE;
			partial_data = true;
			break;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"LicenseStatus");

		if (wmi_res == SUCCESS) {
			os_license = true;
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<int64_t>(stored_property)) {
				switch (std::get<int64_t>(stored_property)) {
				case 0:
					operating_system_details.activation_status = ActivationStatus::UNLICENSED;
					break;
				case 1:
					operating_system_details.activation_status = ActivationStatus::LICENSED;
					break;
				case 2:
					operating_system_details.activation_status = ActivationStatus::OOB_GRACE;
					break;
				case 3:
					operating_system_details.activation_status = ActivationStatus::OOT_GRACE;
					break;
				case 4:
					operating_system_details.activation_status = ActivationStatus::NON_GENUINE_GRACE;
					break;
				case 5:
					operating_system_details.activation_status = ActivationStatus::NOTIFICATION;
					break;
				case 6:
					operating_system_details.activation_status = ActivationStatus::EXTENDED_GRACE;
					break;
				default:
					operating_system_details.activation_status = ActivationStatus::UNAVAILABLE;
					break;
				}
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			operating_system_details.activation_status = ActivationStatus::UNAVAILABLE;
			partial_data = true;
			break;
		}
		else {
			return wmi_res;
		}

	} while (wmi_query_manager.nextWMIObject() == SUCCESS && !os_license);

	if (partial_data) {
		return WARNING_WMI_PARTIAL_DATA;
	}
	else {
		return SUCCESS;
	}
}