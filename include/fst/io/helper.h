//#pragma once
//
//#include "tk/config.hh"
//
//#ifdef __TK_STDLIB__
//#include <iostream>
//#endif  // __TK_STDLIB__
//
//#include <sys/socket.h>  // sockaddr.
//#include <arpa/inet.h>   // inet_ntop.
//
// namespace tk {
// namespace asio {
//
//// Returns true on success, or false if there was an error.
// bool SetSocketBlockingEnabled(int fd, bool blocking);
//
// namespace ip {
// void* GetInAddr(sockaddr* sa);
//
///**
// *
// */
// class Address {
// public:
//  Address();
//
//  Address(const char* addr);
//
//  char* GetData() { return &_addr[0]; }
//
//  const char* GetData() const { return &_addr[0]; }
//
//#ifdef __TK_STDLIB__
//  inline friend std::ostream& operator<<(std::ostream& stream, const Address&
//  addr) { return stream <<
//  addr._addr; }
//#endif  // __TK_STDLIB__
//
// private:
//  char _addr[INET6_ADDRSTRLEN];
//};
//}
//}  // asio.
//}  // tk.
