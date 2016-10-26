#pragma once

#include "../../print.h"
#include "../dispatcher.h"

#include <fcntl.h>
#include <netdb.h> // addrinfo.
#include <signal.h> // sigaction.
#include <string.h>
#include <sys/socket.h> // sockaddr.
#include <sys/un.h> // sockaddr.

#include "../stream/reader.h"
//#include "tk/asio/stream/sender.hh"

namespace fst {
namespace io {
	namespace local {

		class socket {
		public:
			socket(dispatcher* io_dispatcher)
				: _io_dispatcher(io_dispatcher)
				, _reader(io_dispatcher)
			{
			}

			//~socket();

			inline int get_fd() const
			{
				return _sock_fd;
			}

			inline bool is_valid() const
			{
				return _is_valid;
			}

			long async_read(void* data, std::size_t data_size, stream::Reader<socket>::ReadCallback callback,
				void* user_data = nullptr)
			{
				return _reader.AsyncRead(this, _sock_fd, data, data_size, callback, user_data);
			}
			//
			//			long async_write(void* data, std::size_t data_size,
			// stream::Sender<Socket>::SendCallback
			// callback,
			//				void* user_data = nullptr)
			//			{
			//			}

			void close()
			{
			}

			int init(const char* addr)
			{
				_sock_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);

				if (_sock_fd == -1) {
					errprint("Can't create tcp::Socket.");
					return -1;
				}

				sockaddr_un server_addr;
				memset(&server_addr, 0, sizeof(server_addr));
				server_addr.sun_family = AF_UNIX;
				strncpy(server_addr.sun_path, addr, sizeof(server_addr.sun_path));
				server_addr.sun_path[sizeof(server_addr.sun_path) - 1] = '\0';

				//				std::size_t size = (offsetof(struct sockaddr_un, sun_path)
				//+
				// strlen(server_addr.sun_path));

				//				sockaddr_in server_addr;
				//				server_addr.sin_family = AF_INET;
				//				server_addr.sin_port = htons(_port);
				//				server_addr.sin_addr.s_addr = inet_addr(addr);
				//				memset(server_addr.sin_zero, '\0',
				// sizeof(server_addr.sin_zero));

				socklen_t addr_size = sizeof(sockaddr_in);
				int ret = connect(_sock_fd, (struct sockaddr*)&server_addr, addr_size);

				if (ret == -1) {
					errprint("Can't connect tcp::Socket :", std::strerror(errno));

					return -1;
				}

				//				if(bind(_sock_fd, (struct sockaddr*)& server_addr,
				// sizeof(server_addr)) == -1)
				//{
				//					errprint("Can't bind local::Socket",
				// strerror(errno));
				//					return 9;
				//				}

				//				if(bind(_sock_fd, (struct sockaddr*)& server_addr,
				// sizeof(server_addr)) == -1)
				//{
				//					errprint("Can't bind local::Socket",
				// strerror(errno));
				//					return 9;
				//				}

				//				int listen_error = listen(_sock_fd, 20);
				//
				//				if (listen_error == -1) {
				////#ifdef __TK_CONSOLE__
				////					console::Error(TK_TRACER, "listen :",
				/// std::strerror(errno));
				////					console::FError<console::FLogType::ASIO>(TK_TRACER, "listen
				///:",
				/// std::strerror(errno));
				////#endif  // __TK_CONSOLE__
				//					return 2;
				//				}

				//				server_addr.sin_family = AF_UNIX;
				//				server_addr.sin_port = htons(_port);
				//				server_addr.sin_addr.s_addr = inet_addr(addr);
				//				memset(server_addr.sin_zero, '\0',
				// sizeof(server_addr.sin_zero));
				//
				//				socklen_t addr_size = sizeof(sockaddr_in);
				//				int ret = connect(_sock_fd, (struct sockaddr*)&server_addr,
				// addr_size);
				//
				//				if (ret == -1) {
				//					errprint("Can't connect tcp::Socket :",
				// std::strerror(errno));
				//					return -1;
				//				}

				int flags = fcntl(_sock_fd, F_GETFL, 0);
				if (flags < 0) {
					return 3;
				}
				flags = (flags & ~O_NONBLOCK);
				;
				if (fcntl(_sock_fd, F_SETFL, flags) != 0) {
					errprint("Can't set local::Socket to non-blocking.");
					return 4;
				}

				return 0;
			}

		private:
			int _sock_fd = -1;
			int _port = -1;
			bool _is_valid = 0;
			dispatcher* _io_dispatcher = nullptr;
			stream::Reader<socket> _reader;
			//  stream::Sender<socket> _sender;

			// int Init(const char* addr);
		};

	} // local.
} // io.
} // fst.
