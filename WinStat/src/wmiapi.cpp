#define _WIN32_DCOM
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <wbemidl.h>
#include <Windows.h>
#include <comutil.h>
#include <cstdint>
#include "winstat/wmiapi.h"

class WMIQueryManager::WMIManager {
public:
	WMIManager() {
		wmi_initialized_ = false;
		wmi_services_ = nullptr;
		wmi_class_enumerator_ = nullptr;

		wmi_class_object_ = nullptr;
		wmi_class_set_ = false;
	}

	~WMIManager() {
		if (wmi_services_ != nullptr) {
			wmi_services_->Release();
			wmi_services_ = nullptr;
		}

		if (wmi_class_enumerator_ != nullptr) {
			wmi_class_enumerator_->Release();
			wmi_class_enumerator_ = nullptr;
		}

		if (wmi_class_object_ != nullptr) {
			wmi_class_object_->Release();
			wmi_class_object_ = nullptr;
		}

		if (wmi_initialized_) {
			CoUninitialize();
		}
	}

	int setWMIClass(std::wstring query_string, bool initialize_first_object = true) {
		if (!wmi_initialized_) {
			int init_res = initializeWMI();

			if (init_res != SUCCESS) {
				return init_res;
			}
		}

		HRESULT res = NOERROR;

		if (wmi_class_set_) {
			wmi_class_set_ = false;
		}

		res = wmi_services_->ExecQuery(
			_bstr_t(L"WQL"),
			_bstr_t(query_string.c_str()),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&wmi_class_enumerator_);

		if (FAILED(res)) {
			return ERROR_WMI_QUERY_FAILED;
		}

		if (initialize_first_object) {
			ULONG query_res = 0;
			res = wmi_class_enumerator_->Next(WBEM_INFINITE, 1, &wmi_class_object_, &query_res);

			if (FAILED(res)) {
				return ERROR_WMI_NO_MORE_OBJECTS;
			}

			wmi_class_set_ = true;
		}

		return SUCCESS;
	}

	int queryWMIProperty(std::wstring property_name) {
		if (!wmi_initialized_) {
			int init_res = initializeWMI();

			if (init_res != SUCCESS) {
				return init_res;
			}
		}

		if (!wmi_class_set_) {
			return WARNING_WMI_CLASS_NOT_SET;
		}

		HRESULT res = NOERROR;

		VARIANT property_variant;
		VariantInit(&property_variant);

		res = wmi_class_object_->Get(property_name.c_str(), 0, &property_variant, 0, 0);

		if (FAILED(res)) {
			return ERROR_WMI_PROPERTY_NOT_FOUND;
		}

		switch (property_variant.vt) {
		case VT_BSTR:
			if (property_variant.bstrVal != nullptr) {
				wmi_property_.emplace<std::wstring>(std::wstring(property_variant.bstrVal));
			}
			else {
				wmi_property_.emplace<std::wstring>(L"");
			}
			break;
		case VT_UI1:
			wmi_property_.emplace<uint64_t>(static_cast<uint64_t>(property_variant.bVal));
			break;
		case VT_I1:
			wmi_property_.emplace<int64_t>(static_cast<int64_t>(property_variant.cVal));
			break;
		case VT_UI2:
			wmi_property_.emplace<uint64_t>(static_cast<uint64_t>(property_variant.uiVal));
			break;
		case VT_I2:
			wmi_property_.emplace<int64_t>(static_cast<int64_t>(property_variant.iVal));
			break;
		case VT_UI4:
			wmi_property_.emplace<uint64_t>(static_cast<uint64_t>(property_variant.uintVal));
			break;
		case VT_I4:
			wmi_property_.emplace<int64_t>(static_cast<int64_t>(property_variant.lVal));
			break;
		case VT_UI8:
			wmi_property_.emplace<uint64_t>(static_cast<uint64_t>(property_variant.ullVal));
			break;
		case VT_I8:
			wmi_property_.emplace<int64_t>(static_cast<int64_t>(property_variant.llVal));
			break;
		case VT_BOOL:
			wmi_property_.emplace<bool>(property_variant.boolVal == VARIANT_TRUE);
			break;
		case VT_NULL:
			return WARNING_WMI_PARTIAL_DATA;
			break;
		default:
			return ERROR_WMI_PROPERTY_TYPE_NOT_SUPPORTED;
			break;
		}

		VariantClear(&property_variant);

		return SUCCESS;
	}

	std::variant<std::wstring, int64_t, uint64_t, bool> getStoredProperty() const {
		return wmi_property_;
	}

	int nextWMIObject() {
		if (wmi_class_set_) {
			wmi_class_set_ = false;
		}

		HRESULT res = NOERROR;
		ULONG query_res = 0;
		res = wmi_class_enumerator_->Next(WBEM_INFINITE, 1, &wmi_class_object_, &query_res);

		if (FAILED(res) || query_res == 0) {
			return ERROR_WMI_NO_MORE_OBJECTS;
		}

		wmi_class_set_ = true;

		return SUCCESS;
	}

private:
	bool wmi_initialized_;
	IWbemServices* wmi_services_;
	IEnumWbemClassObject* wmi_class_enumerator_;

	IWbemClassObject* wmi_class_object_;
	bool wmi_class_set_;
	std::variant<std::wstring, int64_t, uint64_t, bool> wmi_property_;

	int initializeWMI() {
		HRESULT res = NOERROR;

		res = CoInitializeEx(0, COINIT_MULTITHREADED);

		if (FAILED(res)) {
			return ERROR_WMI_INIT_FAILED;
		}

		wmi_initialized_ = true;

		res = CoInitializeSecurity(
			NULL,
			-1,
			NULL,
			NULL,
			RPC_C_AUTHN_LEVEL_DEFAULT,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE,
			NULL);

		if (FAILED(res)) {
			return ERROR_WMI_SEC_FAILED;
		}

		IWbemLocator* wmi_locator = nullptr;

		res = CoCreateInstance(
			CLSID_WbemLocator,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator,
			(LPVOID*)&wmi_locator);

		if (FAILED(res)) {
			return ERROR_WMI_LOCATOR_FAILED;
		}

		res = wmi_locator->ConnectServer(
			_bstr_t(L"ROOT\\CIMV2"),
			NULL,
			NULL,
			0,
			NULL,
			0,
			0,
			&wmi_services_);

		if (FAILED(res)) {
			wmi_locator->Release();
			wmi_locator = nullptr;

			return ERROR_WMI_CONN_FAILED;
		}

		wmi_locator->Release();
		wmi_locator = nullptr;

		res = CoSetProxyBlanket(
			wmi_services_,
			RPC_C_AUTHN_WINNT,
			RPC_C_AUTHZ_NONE,
			NULL,
			RPC_C_AUTHN_LEVEL_CALL,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE);

		if (FAILED(res)) {
			return ERROR_WMI_PROXY_FAILED;
		}

		return SUCCESS;
	}
};

WMIQueryManager::WMIQueryManager() {
	impl_ = new WMIManager;
}

WMIQueryManager::~WMIQueryManager() {
	delete impl_;
}

int WMIQueryManager::setWMIClass(std::wstring query_string, bool initialize_first_object) {
	return impl_->setWMIClass(query_string, initialize_first_object);
}

int WMIQueryManager::queryWMIProperty(std::wstring property_name) {
	return impl_->queryWMIProperty(property_name);
}

std::variant<std::wstring, int64_t, uint64_t, bool> WMIQueryManager::getStoredProperty() const {
	return impl_->getStoredProperty();
}

int WMIQueryManager::nextWMIObject() {
	return impl_->nextWMIObject();
}