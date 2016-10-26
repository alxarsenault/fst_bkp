#pragma once

#include <cstring>
#include <arpa/inet.h> // ??? INET6_ADDRSTRLEN.

namespace fst {
namespace io {
	namespace ip {
		class address {
		public:
			address()
			{
			}

			address(const char* addr)
			{
				std::strcpy(_addr, addr);
			}

			static void* get_in_addr(sockaddr* sa)
			{
				// IPv4.
				if (sa->sa_family == AF_INET) {
					sockaddr_in* sa_in = (sockaddr_in*)sa;
					return &sa_in->sin_addr;
				}

				// IPv6.
				sockaddr_in6* sa_in6 = (sockaddr_in6*)sa;
				return &sa_in6->sin6_addr;
			}

			char* data()
			{
				return &_addr[0];
			}

			const char* data() const
			{
				return &_addr[0];
			}

		private:
			char _addr[INET6_ADDRSTRLEN];
		};

	} // ip.
} // io.
} // fst.
