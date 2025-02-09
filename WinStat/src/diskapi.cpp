#include "winstat/diskapi.h"
#include "winstat/wmiapi.h"

int queryDiskDetails(std::vector<DiskDetails>& disk_details) {
	std::variant<std::wstring, int64_t, uint64_t, bool> stored_property;
	WMIQueryManager wmi_query_manager;
	int wmi_res = SUCCESS;

	bool partial_data = false;

	wmi_res = wmi_query_manager.setWMIClass(L"SELECT Model, SerialNumber, Size, MediaType, Status FROM Win32_DiskDrive");

	if (wmi_res != SUCCESS) {
		return wmi_res;
	}

	do {
		DiskDetails disk;

		wmi_res = wmi_query_manager.queryWMIProperty(L"Model");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				disk.model = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			disk.model = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"SerialNumber");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				disk.serial_number = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			disk.serial_number = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"Size");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			/*
			The Size property is documented as uint64(VT_UI8),
			But WMI returns the property as BSTR (VT_BSTR).
			*/
			if (std::holds_alternative<std::wstring>(stored_property)) {
				disk.size = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			disk.size = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"MediaType");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				disk.media_type = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			disk.media_type = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"Status");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				disk.status = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			disk.status = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		disk_details.push_back(disk);

	} while (wmi_query_manager.nextWMIObject() == SUCCESS);

	if (partial_data) {
		return WARNING_WMI_PARTIAL_DATA;
	}
	else {
		return SUCCESS;
	}
}