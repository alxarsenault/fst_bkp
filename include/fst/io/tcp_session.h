//#pragma once
//
//#include <cstddef>
//#include "tk/asio/stream/reader.hh"
//#include "tk/asio/stream/sender.hh"
//
// namespace tk {
// namespace asio {
// class Dispatcher;
//
// namespace tcp {
// class Session {
// public:
//  Session(Dispatcher* dispatcher, int sock_fd);
//  virtual ~Session();
//
//  long AsyncRead(void* data, std::size_t data_size,
//  stream::Reader<Session>::ReadCallback callback,
//                 void* user_data = nullptr);
//
//  long AsyncWrite(void* data, std::size_t data_size,
//  stream::Sender<Session>::SendCallback callback,
//                  void* user_data = nullptr);
//
//  int GetSockFd() const { return _sock_fd; }
//
//  inline bool IsValid() const { return _sock_fd != -1; }
//
//  void Close();
//
//  void CloseWrite();
//
// private:
//  int _sock_fd;
//  Dispatcher* _dispatcher;
//  stream::Reader<Session> _reader;
//  stream::Sender<Session> _sender;
//};
//}  // tcp.
//}  // asio.
//}  // tk.
