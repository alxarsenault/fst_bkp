//#pragma once
//
//#include <sys/socket.h>  // sockaddr.
//#include <netdb.h>       // addrinfo.
//#include <signal.h>      // sigaction.
//
//#include "tk/asio/stream/reader.hh"
//#include "tk/asio/stream/sender.hh"
//
//namespace tk {
//namespace asio {
//class Dispatcher;
//namespace tcp {
//
//class Socket {
// public:
//  Socket(Dispatcher* dispatcher, const char* addr, int port);
//
//  ~Socket();
//
//  inline int GetSockFd() const { return _sock_fd; }
//
//  inline bool IsValid() const { return _is_valid == 1; }
//
//  long AsyncRead(void* data, std::size_t data_size, stream::Reader<Socket>::ReadCallback callback,
//                 void* user_data = nullptr);
//
//  long AsyncWrite(void* data, std::size_t data_size, stream::Sender<Socket>::SendCallback callback,
//                  void* user_data = nullptr);
//
//  void Close();
//
// private:
//  int _sock_fd;
//  int _port;
//  int _is_valid;
//  Dispatcher* _dispatcher;
//  stream::Reader<Socket> _reader;
//  stream::Sender<Socket> _sender;
//
//  int Init(const char* addr);
//};
//
//}  // tcp.
//}  // asio.
//}  // tk.
