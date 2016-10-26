#pragma once

#include "../print.h"
#include <fcntl.h>

namespace fst {
namespace io {
	static inline int set_non_blocking(int fd)
	{
		int flags = fcntl(fd, F_GETFL, 0);
		if (flags < 0) {
			return 4;
		}

		flags = flags | O_NONBLOCK;

		if (fcntl(fd, F_SETFL, flags) != 0) {
			errprint(ptrace, "Can't set to non blocking :", std::strerror(errno));
			return 5;
		}

		return 0;
	}
} // io.
} // fst.
