//#pragma once
//
//#include <sys/socket.h>  // sockaddr.
//#include <netdb.h>       // addrinfo.
//#include <signal.h>      // sigaction.
//
//#include "tk/asio/stream/sender.hh"
//
// namespace tk {
// namespace asio {
// class Dispatcher;
// namespace udp {
///**
// * @brief UDP socket.
// */
// class Broadcast {
// public:
//  Broadcast(Dispatcher* dispatcher, int port);
//
//  ~Broadcast();
//
//  inline int GetSockFd() const { return _sock_fd; }
//
//  inline bool IsValid() const { return _is_valid == 1; }
//
//  //  typedef void (*WriteCallback)(tk::asio::udp::Broadcast*, long);
//  //  long AsyncWrite(const void* data, std::size_t data_size, WriteCallback callback);
//  //  static void WriteHandler(void*);
//
//  long AsyncWrite(void* data, std::size_t data_size, stream::Sender<Broadcast>::SendCallback callback,
//                  void* user_data = nullptr);
//
//  void CloseWrite();
//
// private:
//  int _sock_fd;
//  int _port;
//  int _is_valid;
//  //  WriteCallback _write_callback;
//  //  struct sigaction _sig_action;
//
//  Dispatcher* _dispatcher;
//  stream::Sender<Broadcast> _sender;
//
//  int Init();
//};
//
//}  // tcp.
//}  // asio.
//}  // tk.
