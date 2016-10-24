//#pragma once
//
//#include "tk/asio/helper.hh"
//#include <cstdint>
//#include <sys/socket.h>  // sockaddr.
//#include <netdb.h>       // addrinfo.
//#include <signal.h>      // sigaction.
//
// namespace tk {
// namespace asio {
// class Dispatcher;
// namespace tcp {
//
// class Acceptor {
// public:
//  Acceptor(Dispatcher* dispatcher, std::uint16_t port);
//
//  ~Acceptor();
//
//  inline int GetSockFd() const { return _sockfd; }
//
//  inline bool IsValid() const { return _is_valid == 1; }
//
//  typedef void (*AcceptCallback)(tk::asio::Dispatcher* dispatcher, int socket, const ip::Address& ip_addr,
//  void* data);
//
//  void AsyncAccept(AcceptCallback callback, void* data);
//
//  static void ConnectionHanlder(void*);
//
// private:
//  int _sockfd;
//  int _is_valid;
//  int _option;
//  struct sigaction _sig_action;
//  Dispatcher* _dispatcher;
//  std::uint16_t _port;
//
//  AcceptCallback _accept_callback = nullptr;
//  void* _accept_callback_data = nullptr;
//
//  int Init();
//  int BindSockFd(addrinfo* servinfo);
//  addrinfo SetupHintsInfo();
//
//  static const int PENDING_CONNECTION_QUEUE_SIZE;
//};
//
//}  // tcp.
//}  // asio.
//}  // tk.
