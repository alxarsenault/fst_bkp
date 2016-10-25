#pragma once

#include <netdb.h> // addrinfo.
#include <signal.h> // sigaction.
#include <sys/socket.h> // sockaddr.

//#include "tk/asio/stream/reader.hh"
//#include "tk/asio/stream/sender.hh"

namespace fst {
namespace io {
	namespace tcp {

		class socket {
		public:
			socket(dispatcher* io_dispatcher, const char* addr, int port)
			{
			}

			~socket()
			{
			}

			int init(const char* addr)
			{
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

			//  long async_read(void* data, std::size_t data_size,
			//  stream::Reader<Socket>::ReadCallback
			//  callback,
			//		void* user_data = nullptr);

			//  long async_write(void* data, std::size_t data_size,
			//  stream::Sender<Socket>::SendCallback
			//  callback,
			//                  void* user_data = nullptr);

			void close()
			{
			}

		private:
			int _sock_fd;
			int _port;
			int _is_valid;
			dispatcher* _io_dispatcher;
			//  stream::Reader<Socket> _reader;
			//  stream::Sender<Socket> _sender;
		};

	} // tcp.
} // io.
} // fst.
