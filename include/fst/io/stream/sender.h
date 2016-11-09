#pragma once

#include "../../print.h"
#include "../dispatcher.h"
#include "../status.h"

#include <unistd.h>
#include <errno.h>
#include <cstring>

namespace fst {
namespace io {
	namespace stream {

		template <typename T>
		class sender {
		public:
			typedef void (*SendCallback)(T*, void*, status, long);

			sender()
			{
			}

			static void SenderHandler(void* data)
			{
				sender* snder = (sender*)data;

#ifdef __APPLE__
				snder->_n_bytes = send(snder->_fd, snder->_data, snder->_data_size, 0);
#else
				snder->_n_bytes = send(snder->_fd, snder->_data, snder->_data_size, MSG_NOSIGNAL);
#endif

				/// Could not send data.
				if (snder->_n_bytes == -1) {
					// Resource temporarily unavailable.
					if (errno == EAGAIN) {
						errprint(ptrace, "Resource is still unavailable (THIS SHOULD NEVER HAPPEND).");
						return;
					}
					// Broken pipe.
					else if (errno == EPIPE) {
						errprint(ptrace, "Broken pipe.");
						snder->CallUserCallback(status{ state::bad, error::kBrokenPipe });
						return;
					}

					// Unhandled error (for now).
					errprint(ptrace, "Could not send to socket :", std::strerror(errno));
					snder->CallUserCallback(status{ state::bad, error::kUnknown });
					return;
				}

				if (snder->_n_bytes == 0) {
					/// @todo Don't know exactly what to do there.
					warnprint(ptrace, "Could not send to socket again ( write size :", 0, ").",
						std::strerror(errno));
					snder->CallUserCallback(status{ state::bad, error::kReaderWasClosed });
					return;
				}

				snder->CallUserCallback(status{ state::good, error::kNone });
			}

			long async_write(T* owner, int fd, void* const data, std::size_t data_size, SendCallback callback,
				void* user_data)
			{
				_fd = fd;
				_owner = owner;
				_data_size = data_size;
				_data = data;
				_callback = callback;
				_user_data = user_data;

				if (!_callback) {
					warnprint(ptrace, "Sender callback wasn't set.");
					return -1;
				}

				// Invalid file descriptor.
				if (_fd == -1) {
					errprint(ptrace, "Try to send on empty socket.");
					_n_bytes = -1;
					return CallUserCallback(status{ state::bad, error::kInvalidFileDescriptor });
				}

#ifdef __APPLE__
				_n_bytes = send(_fd, data, data_size, 0);
#else
				_n_bytes = send(_fd, data, data_size, MSG_NOSIGNAL);
#endif

				// If could not send data.
				if (_n_bytes == -1) {
					// Resource temporarily unavailable.
					if (errno == EAGAIN) {
						// Add output handler to io dispatcher.
						owner->get_dispatcher()->add_handler(
							_fd, &sender::SenderHandler, this, dispatcher::handle_type::output);
						return _n_bytes;
					}
					// Broken pipe.
					else if (errno == EPIPE) {
						errprint(ptrace, "Broken pipe.");
						return CallUserCallback(status{ state::bad, error::kBrokenPipe });
					}

					// Unhandled error (for now).
					errprint(ptrace, "Write :", std::strerror(errno));
					return CallUserCallback(status{ state::bad, error::kUnknown });
				}

				if (_n_bytes == 0) {
					/// @todo Don't know exactly what to do there.
					/// @todo Add timeout ???
					errprint(ptrace, "@@@@TODO MAYBE ADD TIMEOUT ??? Socket got disconnected ?????");
					return CallUserCallback(status{ state::bad, error::kReaderWasClosed });
				}

				return CallUserCallback(status{ state::good, error::kNone });
			}

		private:
			long _n_bytes = 0;
			int _fd = -1;
			std::size_t _data_size = 0;
			SendCallback _callback = nullptr;
			void* _data = nullptr;
			void* _user_data = nullptr;
			T* _owner = nullptr;

			long CallUserCallback(status sts)
			{
				_callback(_owner, _user_data, sts, _n_bytes);
				return _n_bytes;
			}
		};

	} // stream.
} // io.
} // fst.
