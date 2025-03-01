#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <stdexcept>
#include <wchar.h>
#include "winstat/stringapi.h"

std::wstring stringapi::trim(const std::wstring& string, stringapi::TrimMode trim_mode) {
	if (string.empty()) { return L""; }

	std::wstring string_buffer = string;

	switch (trim_mode) {
	case stringapi::TrimMode::LEFT:
		while (string_buffer.length() != 0) {
			if (string_buffer[0] == L' ') {
				string_buffer.erase(string_buffer.begin());
			}
			else { break; }
		}

		break;
	case stringapi::TrimMode::RIGHT:
		while (string_buffer.length() != 0) {
			if (string_buffer[string_buffer.length() - 1] == L' ') {
				string_buffer.erase(string_buffer.end() - 1);
			}
			else { break; }
		}

		break;
	case stringapi::TrimMode::BOTH:
		while (string_buffer.length() != 0) {
			if (string_buffer[0] == L' ') {
				string_buffer.erase(string_buffer.begin());
			}
			else { break; }
		}

		while (string_buffer.length() != 0) {
			if (string_buffer[string_buffer.length() - 1] == L' ') {
				string_buffer.erase(string_buffer.end() - 1);
			}
			else { break; }
		}

		break;
	default:
		throw std::invalid_argument("Invalid TrimMode value.");
		break;
	}

	return string_buffer;
}

std::wstring stringapi::transformCase(const std::wstring& string, stringapi::CaseMode case_mode) {
	if (string.empty()) { return L""; }

	wchar_t* string_buffer = new wchar_t[string.length() + 1];
	wcscpy_s(
		string_buffer,
		static_cast<unsigned long long>(string.length() + 1),
		string.c_str());

	if (case_mode == stringapi::CaseMode::UPPER) {
		CharUpperW(string_buffer);
	}
	else if (case_mode == stringapi::CaseMode::LOWER) {
		CharLowerW(string_buffer);
	}
	else {
		throw std::invalid_argument("Invalid CaseMode value.");
	}

	std::wstring result = std::wstring(string_buffer);
	delete[] string_buffer;
	return result;
}

bool stringapi::startsWith(const std::wstring& string, const std::wstring& prefix) {
	if (prefix.length() > string.length()) { return false; }
	if (string.empty() && !prefix.empty()) { return false; }
	if (string.empty() && prefix.empty()) { return true; }

	for (int i = 0; i < prefix.length(); i++) {
		if (prefix[i] != string[i]) { return false; }
	}

	return true;
}

bool stringapi::endsWith(const std::wstring& string, const std::wstring& suffix) {
	if (suffix.length() > string.length()) { return false; }
	if (string.empty() && !suffix.empty()) { return false; }
	if (string.empty() && suffix.empty()) { return true; }

	int index_offset = string.length() - suffix.length();
	for (int i = suffix.length() - 1; i >= 0; i--) {
		if (suffix[i] != string[index_offset + i]) { return false; }
	}

	return true;
}