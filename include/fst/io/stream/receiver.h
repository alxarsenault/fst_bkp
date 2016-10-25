//#pragma once
//
//#include "tk/asio/dispatcher.hh"
//#include "tk/asio/helper.hh"
//#include "tk/console/print.hh"
//#include "tk/console/file_print.hh"
//
//#include <unistd.h>
//#include <arpa/inet.h>  // inet_ntop.
//#include <errno.h>
//#include <cstring>
//
//#ifdef __TK_CONSOLE__
//#define TK_CONSOLE(x) x
//#else
//#define TK_CONSOLE(x)
//#endif  // __TK_CONSOLE__
//
// namespace tk {
// namespace asio {
//
// class Dispatcher;
// namespace stream {
// template <typename T>
// class Receiver {
// public:
//  Receiver(Dispatcher* dispatcher) : _dispatcher(dispatcher) {}
//
//  /**
//   *
//   */
//  static void ReceiverHandler(void* data) {
//    Receiver* reader = (Receiver*)data;
//
//    // Could not read from file descriptor.
//    if (reader->_status == -1) {
//      // Try to read again.
//      sockaddr_storage their_addr;
//      socklen_t sin_size = sizeof(their_addr);
//      sockaddr* addr = (sockaddr*)&their_addr;
//      reader->_status = recvfrom(reader->_fd, reader->_data,
//      reader->_data_size, 0, addr, &sin_size);
//
//      char s[INET6_ADDRSTRLEN];
//      inet_ntop(their_addr.ss_family, ip::GetInAddr(addr), s, sizeof(s));
//      TK_CONSOLE(console::FLog<console::FLogType::ASIO>(TK_TRACER, "Receive
//      from :", std::string(s));)
//
//      if (reader->_status == -1) {
//        // Resource temporarily unavailable (which is normal since socket is
//        on non-blocking mode).
//        if (errno == EAGAIN) {
//          TK_CONSOLE(console::Error(TK_TRACER, "Resource is still unavailable
//          (THIS SHOULD NEVER
//          HAPPEND).");)
//          TK_CONSOLE(console::FError<console::FLogType::ASIO>(
//                         TK_TRACER, "Resource is still unavailable (THIS
//                         SHOULD NEVER HAPPEND).");)
//          return;
//        } else if (errno == EAFNOSUPPORT) {  // Address family not supported
//        by protocol family.
//          TK_CONSOLE(console::FWarning<console::FLogType::ASIO>(TK_TRACER,
//                                                                "Address
//                                                                family not
//                                                                supported by
//                                                                protocol
//                                                                family.");)
//          return;
//        }
//
//        TK_CONSOLE(console::Error(TK_TRACER, "Could not read from socket :",
//        std::strerror(errno));)
//        TK_CONSOLE(
//            console::FError<console::FLogType::ASIO>(TK_TRACER, "Could not
//            read from socket :",
//            std::strerror(errno));)
//
//        return;
//      }
//
//      if (reader->_status == 0) {
//        // Don't know what to do there.
//        TK_CONSOLE(console::Warning(TK_TRACER, "Could not read from socket
//        again ( read size :", 0, ").");)
//        TK_CONSOLE(console::FWarning<console::FLogType::ASIO>(
//                       TK_TRACER, "Could not read from socket again ( read
//                       size :", 0, ").");)
//
//        return;
//      }
//
//      if (reader->_callback) {
//        return reader->_callback(reader->_owner, reader->_user_data,
//        ip::Address(s), reader->_status);
//      }
//
//      TK_CONSOLE(console::Warning(TK_TRACER, "Received data but no reader
//      callback was set.");)
//      TK_CONSOLE(
//          console::FWarning<console::FLogType::ASIO>(TK_TRACER, "Received data
//          but no reader callback was
//          set.");)
//    }
//  }
//
//  /**
//   * @brief Read callback function.
//   */
//  typedef void (*ReadCallback)(T*, void*, const ip::Address& addr, long);
//
//  /**
//   *
//   */
//  long AsyncRead(T* owner, int fd, void* data, std::size_t data_size,
//  ReadCallback callback, void*
//  user_data) {
//    _fd = fd;
//
//    if (_fd == -1) {
//      TK_CONSOLE(console::Error(TK_TRACER, "Try to read on empty socket.");)
//      TK_CONSOLE(console::FError<console::FLogType::ASIO>(TK_TRACER, "Try to
//      read on empty socket.");)
//
//      return -1;
//    }
//
//    _owner = owner;
//    _data_size = data_size;
//    _data = data;
//    _callback = callback;
//    _user_data = user_data;
//
//    _dispatcher->AddHandler(_fd, &Receiver::ReceiverHandler, this,
//    Dispatcher::HandleType::Input);
//
//    sockaddr_storage their_addr;
//    socklen_t sin_size = sizeof(their_addr);
//    sockaddr* addr = (sockaddr*)&their_addr;
//    //	struct sockaddr *src_addr
//    _status = recvfrom(_fd, _data, _data_size, 0, addr, &sin_size);
//
//    char s[INET6_ADDRSTRLEN];
//    inet_ntop(their_addr.ss_family, ip::GetInAddr(addr), s, sizeof(s));
//
//    TK_CONSOLE(console::FLog<console::FLogType::ASIO>(TK_TRACER, "Receive from
//    :", std::string(s));)
//
//    if (_status == -1) {
//      // Resource temporarily unavailable (which is normal since socket is on
//      non-blocking mode).
//      if (errno == EAGAIN) {
//        return _status;
//      } else if (errno == EAFNOSUPPORT) {  // Address family not supported by
//      protocol family.
//
//        TK_CONSOLE(
//            console::FWarning<console::FLogType::ASIO>(TK_TRACER, "Address
//            family not supported by protocol
//            family.");)
//
//        return _status;
//      }
//
//      TK_CONSOLE(console::Error(TK_TRACER, "Read :", std::strerror(errno));)
//      TK_CONSOLE(console::FError<console::FLogType::ASIO>(TK_TRACER, "Read :",
//      std::strerror(errno));)
//
//      return _status;
//    }
//
//    if (_status && _callback) {
//      _callback(_owner, _user_data, ip::Address(s), _status);
//      return _status;
//    }
//
//    TK_CONSOLE(console::Warning(TK_TRACER, "Received data but no reader
//    callback was set.");)
//    TK_CONSOLE(console::FWarning<console::FLogType::ASIO>(TK_TRACER, "Received
//    data but no reader callback
//    was set.");)
//
//    return _status;
//  }
//
// private:
//  long _status = 0;
//  int _fd = -1;
//  std::size_t _data_size = 0;
//  ReadCallback _callback = nullptr;
//  void* _data = nullptr;
//  void* _user_data = nullptr;
//  T* _owner = nullptr;
//  Dispatcher* _dispatcher;
//};
//
//}  // stream.
//}  // asio.
//}  // tk.
//
//#undef TK_CONSOLE
