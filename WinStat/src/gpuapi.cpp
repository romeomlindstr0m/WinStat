#include "winstat/gpuapi.h"
#include "winstat/wmiapi.h"

int queryGPUDetails(std::vector<GPUDetails>& gpu_details) {
	int wmi_res = SUCCESS;
	WMIQueryManager wmi_query_manager;
	bool partial_data = false;

	wmi_res = wmi_query_manager.setWMIClass(L"SELECT Name, DriverVersion, MaxMemorySupported, Status, CurrentRefreshRate, MaxRefreshRate FROM Win32_VideoController");

	if (wmi_res != SUCCESS) {
		return wmi_res;
	}

	do {
		std::variant<std::wstring, int64_t, uint64_t, bool> stored_property;
		GPUDetails gpu;

		wmi_res = wmi_query_manager.queryWMIProperty(L"Name");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				gpu.name = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		} 
		else if(wmi_res == WARNING_WMI_PARTIAL_DATA) {
			gpu.name = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"DriverVersion");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				gpu.driver_version = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			gpu.driver_version = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"MaxMemorySupported");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<int64_t>(stored_property)) {
				gpu.total_memory = std::get<int64_t>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			gpu.total_memory = 0;
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"Status");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				gpu.status = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			gpu.status = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"CurrentRefreshRate");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<int64_t>(stored_property)) {
				gpu.current_refresh_rate = std::get<int64_t>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			gpu.current_refresh_rate = 0;
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"MaxRefreshRate");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<int64_t>(stored_property)) {
				gpu.max_refresh_rate = std::get<int64_t>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			gpu.max_refresh_rate = 0;
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		gpu_details.push_back(gpu);

	} while (wmi_query_manager.nextWMIObject() == SUCCESS);

	if (partial_data) {
		return WARNING_WMI_PARTIAL_DATA;
	}
	else {
		return SUCCESS;
	}
}

int queryMonitorDetails(std::vector<MonitorDetails>& monitor_details) {
	int wmi_res = SUCCESS;
	WMIQueryManager wmi_query_manager;
	bool partial_data = false;

	wmi_res = wmi_query_manager.setWMIClass(L"SELECT Name, MonitorManufacturer, ScreenHeight, ScreenWidth, Status FROM Win32_DesktopMonitor");

	if (wmi_res != SUCCESS) {
		return wmi_res;
	}

	do {
		std::variant<std::wstring, int64_t, uint64_t, bool> stored_property;
		MonitorDetails monitor;

		wmi_res = wmi_query_manager.queryWMIProperty(L"Name");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				monitor.name = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			monitor.name = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"MonitorManufacturer");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				monitor.manufacturer = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			monitor.manufacturer = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"ScreenHeight");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<int64_t>(stored_property)) {
				monitor.height = std::get<int64_t>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			monitor.height = 0;
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"ScreenWidth");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<int64_t>(stored_property)) {
				monitor.width = std::get<int64_t>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			monitor.width = 0;
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		wmi_res = wmi_query_manager.queryWMIProperty(L"Status");

		if (wmi_res == SUCCESS) {
			stored_property = wmi_query_manager.getStoredProperty();

			if (std::holds_alternative<std::wstring>(stored_property)) {
				monitor.status = std::get<std::wstring>(stored_property);
			}
			else {
				return ERROR_UNEXPECTED_VARIANT_TYPE;
			}
		}
		else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
			monitor.status = L"";
			partial_data = true;
		}
		else {
			return wmi_res;
		}

		monitor_details.push_back(monitor);

	} while (wmi_query_manager.nextWMIObject() == SUCCESS);

	if (partial_data) {
		return WARNING_WMI_PARTIAL_DATA;
	}
	else {
		return SUCCESS;
	}
}