#include "wsi/wmiapi.h"
#include "wsi/processorapi.h"

int queryProcessorDetails(ProcessorDetails& processor_details) {
	std::variant<std::wstring, int64_t, uint64_t, bool> stored_property;
	WMIQueryManager wmi_query_manager;
	int wmi_res = SUCCESS;

	bool partial_data = false;

	wmi_res = wmi_query_manager.setWMIClass(L"SELECT Manufacturer, Architecture, NumberOfCores, ThreadCount FROM Win32_Processor");

	if (wmi_res != SUCCESS) {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"Manufacturer");

	if (wmi_res == SUCCESS) {
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<std::wstring>(stored_property)) {
			processor_details.vendor = std::get<std::wstring>(stored_property);
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		processor_details.vendor = L"";
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"Architecture");

	if (wmi_res == SUCCESS) {
		/*
		The Architecture property is documented as uint16(VT_UI2),
		But WMI returns the property as int32 (VT_I4).
		*/
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<int64_t>(stored_property)) {
			processor_details.architecture = std::get<int64_t>(stored_property);
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		processor_details.architecture = 0;
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"NumberOfCores");

	if (wmi_res == SUCCESS) {
		/*
		The NumberOfCores property is documented as uint32(VT_UI4),
		But WMI returns the property as int32 (VT_I4).
		*/
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<int64_t>(stored_property)) {
			processor_details.core_count = std::get<int64_t>(stored_property);
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		processor_details.core_count = 0;
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	wmi_res = wmi_query_manager.queryWMIProperty(L"ThreadCount");

	if (wmi_res == SUCCESS) {
		/*
		The ThreadCount property is documented as uint32(VT_UI4),
		But WMI returns the property as int32 (VT_I4).
		*/
		stored_property = wmi_query_manager.getStoredProperty();

		if (std::holds_alternative<int64_t>(stored_property)) {
			processor_details.thread_count = std::get<int64_t>(stored_property);
		}
		else {
			return ERROR_UNEXPECTED_VARIANT_TYPE;
		}
	}
	else if (wmi_res == WARNING_WMI_PARTIAL_DATA) {
		processor_details.thread_count = 0;
		partial_data = true;
	}
	else {
		return wmi_res;
	}

	if (partial_data) {
		return WARNING_WMI_PARTIAL_DATA;
	}
	else {
		return SUCCESS;
	}
}