#pragma once

#include "../../print.h"
#include "../dispatcher.h"
#include "../status.h"

#include <cstring>
#include <errno.h>
#include <unistd.h>

namespace fst {
namespace io {
	namespace stream {

		template <typename T>
		class reader {
		public:
			typedef void (*ReadCallback)(T*, void*, status, long);

			reader()
			{
			}

			static void ReaderHandler(void* data)
			{
				reader* rder = (reader*)data;

				// Could not read from file descriptor.
				// Try to read again.
				rder->_n_bytes = read(rder->_fd, rder->_data, rder->_data_size);

				if (rder->_n_bytes == -1) {
					// Resource temporarily unavailable.
					if (errno == EAGAIN) {
						errprint(ptrace, "Resource is still unavailable (THIS SHOULD NEVER HAPPEND).");
						return;
					}
					// Broken pipe.
					else if (errno == EPIPE) {
						errprint(ptrace, "Broken pipe.");
						rder->CallUserCallback(status{ state::bad, error::kBrokenPipe });
						return;
					}

					// Unhandled error (for now).
					errprint(ptrace, "Could not read from socket :", std::strerror(errno));
					rder->CallUserCallback(status{ state::bad, error::kUnknown });
					return;
				}

				if (rder->_n_bytes == 0) {
					warnprint(ptrace, "Could not read from socket again ( read size :", 0, ").",
						std::strerror(errno));

					rder->CallUserCallback(status{ state::bad, error::kSenderWasClosed });
					return;
				}

				rder->CallUserCallback(status{ state::good, error::kNone });
				return;
			}

			long async_read(
				T* owner, int fd, void* data, std::size_t data_size, ReadCallback callback, void* user_data)
			{
				_fd = fd;
				_owner = owner;
				_data_size = data_size;
				_data = data;
				_callback = callback;
				_user_data = user_data;

				if (!_callback) {
					warnprint(ptrace, "Received data but no callback was set.");
					return -1;
				}

				// Invalid file descriptor.
				if (_fd == -1) {
					errprint(ptrace, "Try to read on empty socket.");
					_n_bytes = -1;
					return CallUserCallback(status{ state::bad, error::kInvalidFileDescriptor });
				}

				_n_bytes = read(_fd, _data, _data_size);

				if (_n_bytes == -1) {
					// Resource temporarily unavailable (which is normal since socket is on non-blocking
					// mode).
					if (errno == EAGAIN) {
						owner->get_dispatcher()->add_handler(
							_fd, &reader::ReaderHandler, this, dispatcher::handle_type::input);
						return _n_bytes;
					}
					// Broken pipe.
					else if (errno == EPIPE) {

						errprint(ptrace, "Broken pipe.");
						return CallUserCallback(status{ state::bad, error::kBrokenPipe });
					}

					// Unhandled error (for now).
					errprint(ptrace, "Read :", std::strerror(errno));
					return CallUserCallback(status{ state::bad, error::kUnknown });
				}

				if (_n_bytes == 0) {
					/// @todo Don't know exactly what to do there.
					warnprint(ptrace, "Could not read from socket again ( read size :", 0, ").",
						std::strerror(errno));
					return CallUserCallback(status{ state::bad, error::kSenderWasClosed });
				}

				return CallUserCallback(status{ state::good, error::kNone });
			}

		private:
			long _n_bytes = 0;
			int _fd = -1;
			std::size_t _data_size = 0;
			ReadCallback _callback = nullptr;
			void* _data = nullptr;
			void* _user_data = nullptr;
			T* _owner = nullptr;

			long CallUserCallback(status status)
			{
				_callback(_owner, _user_data, status, _n_bytes);
				return _n_bytes;
			}
		};

	} // stream.
} // io.
} // fst.
