//#pragma once
//
//#include <sys/socket.h>  // sockaddr.
//#include <netdb.h>       // addrinfo.
//#include <signal.h>      // sigaction.
//
//#include "tk/asio/stream/receiver.hh"
//
// namespace tk {
// namespace asio {
// class Dispatcher;
// namespace udp {
///**
// * @brief UDP socket.
// */
// class BroadcastListener {
// public:
//  BroadcastListener(Dispatcher* dispatcher, int port);
//
//  ~BroadcastListener();
//
//  inline int GetSockFd() const { return _sock_fd; }
//
//  inline bool IsValid() const { return _is_valid == 1; }
//
//  long AsyncRead(void* data, std::size_t data_size,
//  stream::Receiver<BroadcastListener>::ReadCallback
//  callback,
//                 void* user_data = nullptr);
//
//  //  void CloseWrite();
//
// private:
//  int _sock_fd;
//  int _port;
//  int _is_valid;
//  //  WriteCallback _write_callback;
//  //  struct sigaction _sig_action;
//
//  //  Dispatcher* _dispatcher;
//  stream::Receiver<BroadcastListener> _reader;
//
//  int Init();
//};
//
//}  // tcp.
//}  // asio.
//}  // tk.
