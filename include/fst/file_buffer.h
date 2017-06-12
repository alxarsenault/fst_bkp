#pragma once

#include <fst/buffer_view.h>
#include <fst/print.h>

#if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#elif define(_WIN32)
#error fst::file_buffer not implemented on Windows yet.
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa366542(v=vs.85).aspx
#else
#error Your system doesn't have file mapping implementation.
#endif // Unix.

namespace fst {
class file_buffer {
public:
	inline file_buffer(const char* filepath)
	{
		if ((_fd = open(filepath, O_RDONLY)) == -1) {
			_size = 0;
			_data = nullptr;
			_is_valid = false;
			return;
		}

		struct stat st;
		// Get file size;
		if (int status = fstat(_fd, &st) == -1) {
			_size = 0;
			_data = nullptr;
			_is_valid = false;
			return;
		}

		_size = st.st_size;
		_data = (char*)mmap(nullptr, _size, PROT_READ, MAP_PRIVATE, _fd, 0);
		_is_valid = true;
	}

	inline ~file_buffer()
	{
		if (_is_valid) {
			/// @todo Handle error.
			munmap(_data, _size);
		}
	}

	inline bool is_valid() const
	{
		return _is_valid;
	}

	inline fst::buffer_view<char> content() const
	{
		return fst::buffer_view<char>(_data, _size);
	}

private:
	int _fd;
	std::size_t _size;
	char* _data;
	bool _is_valid;
};
} // fst.
