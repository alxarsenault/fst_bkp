#pragma once

#include "../../print.h"
#include "../dispatcher.h"
#include "../fd.h"

#include <netdb.h> // addrinfo.
#include <signal.h> // sigaction.
#include <sys/socket.h> // sockaddr.

#include <arpa/inet.h> // inet_ntop.
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <fcntl.h>

#include "../stream/sender.h"
#include "../stream/reader.h"

namespace fst {
namespace io {
	namespace tcp {

		class socket {
		public:
			socket() = default;

			socket(dispatcher* io_dispatcher)
				: _io_dispatcher(io_dispatcher)
			{
			}

			socket(dispatcher* io_dispatcher, int sock_fd)
				: _sock_fd(sock_fd)
				, _io_dispatcher(io_dispatcher)
			{
				if (_sock_fd != -1) {
					_is_valid = 1;
				}
			}

			~socket()
			{
				close();
			}

			int init(const char* addr, int port)
			{
				_sock_fd = ::socket(AF_INET, SOCK_STREAM, 0);

				if (_sock_fd == -1) {
					errprint(ptrace, "Can't create tcp::Socket.");
					return -1;
				}

				sockaddr_in server_addr;
				server_addr.sin_family = AF_INET;
				server_addr.sin_port = htons(port);
				server_addr.sin_addr.s_addr = inet_addr(addr);
				memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

				socklen_t addr_size = sizeof(sockaddr_in);
				int ret = connect(_sock_fd, (struct sockaddr*)&server_addr, addr_size);

				if (ret == -1) {
					errprint(ptrace, "Can't connect tcp::Socket :", std::strerror(errno));
					return -1;
				}

				if (set_non_blocking(_sock_fd)) {
					errprint(ptrace, "Can't set tcp::Socket to non-blocking.");
					return 8;
				}

				_is_valid = 1;
				return 0;
			}

			inline int get_fd() const
			{
				return _sock_fd;
			}

			inline bool is_valid() const
			{
				return _is_valid == 1;
			}

			long async_read(void* data, std::size_t data_size, stream::reader<socket>::ReadCallback callback,
				void* user_data = nullptr)
			{
				return _reader.async_read(this, _sock_fd, data, data_size, callback, user_data);
			}

			long async_write(void* data, std::size_t data_size, stream::sender<socket>::SendCallback callback,
				void* user_data = nullptr)
			{
				return _sender.async_write(this, _sock_fd, data, data_size, callback, user_data);
			}

			void close()
			{
				if (_sock_fd != -1) {
					::close(_sock_fd);
				}
			}

			dispatcher* get_dispatcher()
			{
				return _io_dispatcher;
			}

			const dispatcher* get_dispatcher() const
			{
				return _io_dispatcher;
			}

		private:
			int _sock_fd = -1;
			int _is_valid = 0;
			dispatcher* _io_dispatcher = nullptr;
			stream::sender<socket> _sender;
			stream::reader<socket> _reader;
		};

	} // tcp.
} // io.
} // fst.
