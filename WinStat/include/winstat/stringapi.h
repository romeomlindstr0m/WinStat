#pragma once
#include <string>
#include "winstat/common.h"

namespace stringapi {
	WINSTAT_API enum class TrimMode {
		LEFT, RIGHT, BOTH
	};

	WINSTAT_API enum class CaseMode {
		UPPER, LOWER
	};

	std::wstring WINSTAT_API trim(const std::wstring& string, TrimMode trim_mode);
	std::wstring WINSTAT_API transformCase(const std::wstring& string, CaseMode case_mode);
}