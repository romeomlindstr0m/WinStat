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
		for (int i = 0; i < string_buffer.length(); i++) {
			if (string_buffer[i] == L' ') {
				string_buffer.erase(string_buffer.begin() + string_buffer[i]);
			}
			else { break; }
		}
		break;
	case stringapi::TrimMode::RIGHT:
		for (int i = string_buffer.length(); i >= 0; i--) {
			if (string_buffer[i] == L' ') {
				string_buffer.erase(string_buffer.begin() + string_buffer[i]);
			}
			else { break; }
		}
		break;
	case stringapi::TrimMode::BOTH:
		for (int i = 0; i < string_buffer.length(); i++) {
			if (string_buffer[i] == L' ') {
				string_buffer.erase(string_buffer.begin() + string_buffer[i]);
			}
			else { break; }
		}

		for (int i = string_buffer.length(); i >= 0; i--) {
			if (string_buffer[i] == L' ') {
				string_buffer.erase(string_buffer.begin() + string_buffer[i]);
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