#pragma once
#include <string>
#include "winstat/common.h"

namespace fsapi {

	class WINSTAT_API File {
	public:
		explicit File(const std::wstring& path);
		~File();

		int write(const std::wstring& buffer);
		int read(std::wstring& buffer);
		int remove();
		bool exists();
		int getSize();
		std::wstring getPath();

	private:
		class FileImpl;
		FileImpl* impl_;
	};

}