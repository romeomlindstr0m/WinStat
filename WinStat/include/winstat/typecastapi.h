#pragma once
#include <string>
#include "winstat/common.h"

namespace typecastapi {
	int WINSTAT_API utf8ToUtf16(const std::string& utf8_string, std::wstring& utf16_string);
	
	int WINSTAT_API utf16ToUtf8(const std::wstring& utf16_string, std::string& utf8_string);
}