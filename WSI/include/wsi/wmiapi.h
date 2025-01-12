#pragma once
#include <string>
#include <variant>
#include "common.h"

class WSI_API WMIQueryManager {
public:
	WMIQueryManager();
	~WMIQueryManager();

	int setWMIClass(std::wstring query_string, bool initialize_first_object = true);
	int queryWMIProperty(std::wstring property_name);
	std::variant<std::wstring, int64_t, uint64_t, bool> getStoredProperty() const;
	int nextWMIObject();
private:
	class WMIManager;
	WMIManager* impl_;
};