#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <vector>
#include "winstat/fsapi.h"
#include "winstat/typecastapi.h"

class fsapi::File::FileImpl {
public:
	explicit FileImpl(const std::wstring& path) {
		file_path_ = path;
		file_handle_ = INVALID_HANDLE_VALUE;
	}
	~FileImpl() {
		if (file_handle_ != INVALID_HANDLE_VALUE) {
			CloseHandle(file_handle_);
		}
	}

	int write(const std::wstring& buffer) {
		if (file_handle_ == INVALID_HANDLE_VALUE) {
			int open_result = openFile();

			if (!IS_SUCCESS(open_result)) {
				return open_result;
			}
		}

		std::string utf8_buffer = "";
		int conversion_result = typecastapi::utf16ToUtf8(buffer, utf8_buffer);

		if (!IS_SUCCESS(conversion_result)) {
			return conversion_result;
		}

		BOOL write_result = WriteFile(
			file_handle_,
			utf8_buffer.c_str(),
			static_cast<DWORD>(utf8_buffer.size()),
			NULL, NULL);

		if (write_result == FALSE) {
			return ERROR_FILE_WRITE_FAILED;
		}

		return SUCCESS;
	}

	int read(std::wstring& buffer) {
		if (file_handle_ == INVALID_HANDLE_VALUE) {
			int open_result = openFile();

			if (!IS_SUCCESS(open_result)) {
				return open_result;
			}
		}

		int bytes_to_read = getSize();

		if (bytes_to_read == -1) {
			return ERROR_FILE_SIZE_READ_FAILED;
		}

		std::vector<char> read_buffer(bytes_to_read);
		DWORD bytes_read = 0;
		BOOL read_result = ReadFile(
			file_handle_,
			read_buffer.data(),
			static_cast<DWORD>(bytes_to_read),
			&bytes_read,
			NULL);

		if (read_result == FALSE) {
			return ERROR_FILE_READ_FAILED;
		}

		std::string utf8_string(read_buffer.begin(), read_buffer.end());
		std::wstring utf16_buffer = L"";
		int conversion_result = typecastapi::utf8ToUtf16(utf8_string, utf16_buffer);

		if (!IS_SUCCESS(conversion_result)) {
			return conversion_result;
		}

		buffer = utf16_buffer;
		return SUCCESS;
	}

	int remove() {
		if (file_handle_ == INVALID_HANDLE_VALUE) {
			int open_result = openFile();

			if (!IS_SUCCESS(open_result)) {
				return open_result;
			}
		}

		BOOL deletion_result = DeleteFileW(file_path_.c_str());

		if (deletion_result == 0) {
			DWORD last_error = GetLastError();

			if (last_error == ERROR_FILE_NOT_FOUND) {
				return ERROR_FILE_NONEXISTENT;
			}
			else if (last_error == ERROR_ACCESS_DENIED) {
				return ERROR_FILE_READONLY;
			}
			else {
				return ERROR_FILE_GENERIC_ERROR;
			}
		}

		return SUCCESS;
	}

	bool exists() {
		if (file_handle_ != INVALID_HANDLE_VALUE) {
			return true;
		}
		
		openFile();
		if (file_handle_ != INVALID_HANDLE_VALUE) {
			return true;
		}
		else {
			return false;
		}
	}

	int getSize() {
		if (file_handle_ == INVALID_HANDLE_VALUE) {
			int open_res = openFile();

			if (!IS_SUCCESS(open_res)) {
				return open_res;
			}
		}

		LARGE_INTEGER file_size{};
		if (GetFileSizeEx(file_handle_, &file_size) == 0) {
			return -1;
		}

		return static_cast<int>(file_size.QuadPart);
	}

	std::wstring getPath() { return file_path_; }

private:
	std::wstring file_path_;
	HANDLE file_handle_;

	int openFile() {
		file_handle_ = CreateFileW(
			file_path_.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (file_handle_ == INVALID_HANDLE_VALUE) {
			if (GetLastError() == ERROR_FILE_NOT_FOUND) {
				return ERROR_FILE_NONEXISTENT;
			}
			else {
				return ERROR_FILE_GENERIC_ERROR;
			}
		}

		return SUCCESS;
	}
};

fsapi::File::File(const std::wstring& path) {
	impl_ = new FileImpl(path);
}

fsapi::File::~File() {
	delete impl_;
}

int fsapi::File::write(const std::wstring& buffer) { return impl_->write(buffer); }

int fsapi::File::read(std::wstring& buffer) { return impl_->read(buffer); }

int fsapi::File::remove() { return impl_->remove(); }

bool fsapi::File::exists() { return impl_->exists(); }

int fsapi::File::getSize() { return impl_->getSize(); }

std::wstring fsapi::File::getPath() { return impl_->getPath(); }