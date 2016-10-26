#pragma once

#include "../../print.h"
#include "../dispatcher.h"
#include "../fd.h"
#include "../ip/address.h"
#include "socket.h"

//#include "tk/asio/helper.hh"
#include <cstdint>
#include <sys/socket.h> // sockaddr.
#include <netdb.h> // addrinfo.
#include <signal.h> // sigaction.

/// @todo Is there something unused ?
#include <unistd.h>
#include <arpa/inet.h> // inet_ntop.
#include <sys/wait.h>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <ifaddrs.h>

#include <arpa/inet.h> // inet_ntop.

namespace fst {
namespace io {
	namespace tcp {

		class acceptor {
		public:
			acceptor(dispatcher* io_dispatcher, std::uint16_t port)
				: _port(port)
				, _io_dispatcher(io_dispatcher)
			{
			}

			~acceptor()
			{
				if (_sockfd != -1) {
					close(_sockfd);
				}
			}

			inline int get_fd() const
			{
				return _sockfd;
			}

			inline bool is_valid() const
			{
				return _is_valid == 1;
			}

			//			typedef void (*accept_callback)(
			//				dispatcher* io_dispatcher, int socket, const ip::address& ip_addr, void* data);

			//			typedef void (*accept_callback)(
			//			dispatcher* io_dispatcher, socket&& sock, const ip::address& ip_addr, void* data);

			typedef void (*accept_callback)(socket&& sock, const ip::address& ip_addr, void* data);

			void async_accept(accept_callback callback, void* data)
			{

				int err = init();

				if (err) {
					errprint(ptrace, "Can't init socket.");
					return;
				}

				_is_valid = 1;
				_accept_callback = callback;
				_accept_callback_data = data;
				_io_dispatcher->add_handler(
					_sockfd, &acceptor::connection_hanlder, this, dispatcher::handle_type::input);
			}

			static void connection_hanlder(void* data)
			{
				acceptor* accpr_socket = (acceptor*)data;
				sockaddr_storage their_addr;
				socklen_t sin_size = sizeof(their_addr);
				sockaddr* addr = (sockaddr*)&their_addr;

				int new_fd = accept(accpr_socket->get_fd(), addr, &sin_size);

				if (new_fd == -1) {
					errprint(ptrace, "Can't do accept.");
					return;
				}

				if (set_non_blocking(new_fd)) {
					/// @todo Close 'new_fd' socket.
					return;
				}

#ifdef __APPLE__
				// Set no sigpipe.
				/// @todo Check error.
				int set = 1;
				setsockopt(new_fd, SOL_SOCKET, SO_NOSIGPIPE, (void*)&set, sizeof(int));
#endif // __APPLE__.

				char s[INET6_ADDRSTRLEN];
				inet_ntop(their_addr.ss_family, ip::address::get_in_addr(addr), s, sizeof(s));

				if (accpr_socket->_accept_callback) {
					accpr_socket->_accept_callback(socket(accpr_socket->_io_dispatcher, new_fd),
						ip::address(s), accpr_socket->_accept_callback_data);
				}
			}

		private:
			int _sockfd = -1;
			int _is_valid = 0;
			std::uint16_t _port;
			struct sigaction _sig_action;
			dispatcher* _io_dispatcher;

			static const int PENDING_CONNECTION_QUEUE_SIZE = 10;

			accept_callback _accept_callback = nullptr;
			void* _accept_callback_data = nullptr;

			int init()
			{
				addrinfo hints;
				memset(&hints, 0, sizeof(hints));
				hints.ai_family = AF_UNSPEC;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_flags = AI_PASSIVE; // Use my IP.

				addrinfo* servinfo = nullptr;

				// Convert port to string.
				char port_str[16];
				std::snprintf(port_str, 16, "%d", _port);

				int rv = getaddrinfo(nullptr, port_str, &hints, &servinfo);

				if (rv != 0) {
					errprint(ptrace, "getaddrinfo :", gai_strerror(rv));
					return 1;
				}

				int binding_error = bind_sock(servinfo, _sockfd, 1);

				if (binding_error != 0) {
					return binding_error;
				}

				// Done with this structure.
				freeaddrinfo(servinfo);

				int listen_error = listen(_sockfd, PENDING_CONNECTION_QUEUE_SIZE);

				if (listen_error == -1) {
					errprint(ptrace, "listen :", std::strerror(errno));
					return 2;
				}

				// Reap all dead processes.
				_sig_action.sa_handler = accpetor_sigchld_handler;
				sigemptyset(&_sig_action.sa_mask);
				_sig_action.sa_flags = SA_RESTART;

				int sigaction_error = sigaction(SIGCHLD, &_sig_action, nullptr);

				if (sigaction_error == -1) {
					errprint(ptrace, "sigaction :", std::strerror(errno));
					return 3;
				}

				if (set_non_blocking(_sockfd)) {
					return 5;
				}

				return 0;
			}

			static int bind_sock(addrinfo* servinfo, int& sock_fd, int option)
			{
				addrinfo* p = nullptr;

				for (p = servinfo; p != nullptr; p = p->ai_next) {
					sock_fd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);

					if (sock_fd == -1) {
						errprint(ptrace, "socket :", std::strerror(errno));
						continue;
					}

					int err = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(int));

					if (err == -1) {
						errprint(ptrace, "setsockopt :", std::strerror(errno));
						return 4;
					}

					err = bind(sock_fd, p->ai_addr, p->ai_addrlen);

					if (err == -1) {
						close(sock_fd);
						errprint(ptrace, "bind :", std::strerror(errno));
						continue;
					}
					break;
				}

				if (p == nullptr) {
					errprint(ptrace, "Can't bind :", std::strerror(errno));
					return 2;
				}

				return 0;
			}

			static void accpetor_sigchld_handler(int s _FST_UNUSED)
			{
				warnprint(ptrace);
				while (waitpid(-1, nullptr, WNOHANG) > 0) {
				};
			}
		};

	} // tcp.
} // io.
} // fst.
