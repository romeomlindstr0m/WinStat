#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "winstat/typecastapi.h"

int typecastapi::utf8ToUtf16(const std::string& utf8_string, std::wstring& utf16_string) {
	int required_buffer_sz = MultiByteToWideChar(
		CP_UTF8,
		MB_ERR_INVALID_CHARS,
		utf8_string.c_str(),
		-1, NULL, 0);

	if (required_buffer_sz == 0) {
		DWORD last_error = GetLastError();

		if (last_error == ERROR_NO_UNICODE_TRANSLATION) {
			return ERROR_INVALID_CHARACTER_ENCOUNTERED;
		}
		else {
			return ERROR_UTF8_BUFFER_FAILED;
		}
	}

	wchar_t* utf16_buffer = new wchar_t[required_buffer_sz];

	int conversion_result = MultiByteToWideChar(
		CP_UTF8,
		MB_ERR_INVALID_CHARS,
		utf8_string.c_str(),
		-1,
		utf16_buffer,
		required_buffer_sz);

	if (conversion_result == 0) {
		delete[] utf16_buffer;
		return ERROR_CONVERSION_FAILED;
	}

	utf16_string = std::wstring(utf16_buffer);
	delete[] utf16_buffer;
	return SUCCESS;
}

int typecastapi::utf16ToUtf8(const std::wstring& utf16_string, std::string& utf8_string) {
	int required_buffer_sz = WideCharToMultiByte(
		CP_UTF8,
		WC_ERR_INVALID_CHARS,
		utf16_string.c_str(),
		-1, NULL, 0, NULL, NULL);

	if (required_buffer_sz == 0) {
		DWORD last_error = GetLastError();

		if (last_error == ERROR_NO_UNICODE_TRANSLATION) {
			return ERROR_INVALID_CHARACTER_ENCOUNTERED;
		}
		else {
			return ERROR_UTF16_BUFFER_FAILED;
		}
	}

	char* utf8_buffer = new char[required_buffer_sz];

	int conversion_result = WideCharToMultiByte(
		CP_UTF8,
		WC_ERR_INVALID_CHARS,
		utf16_string.c_str(),
		-1,
		utf8_buffer,
		required_buffer_sz,
		NULL, NULL);

	if (conversion_result == 0) {
		delete[] utf8_buffer;
		return ERROR_CONVERSION_FAILED;
	}

	utf8_string = std::string(utf8_buffer);
	delete[] utf8_buffer;
	return SUCCESS;
}