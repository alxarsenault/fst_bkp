//#pragma once
//
//#include "tk/asio/dispatcher.hh"
//#include "tk/asio/status.hh"
//#include "tk/console/print.hh"
//#include "tk/console/file_print.hh"
//#include <unistd.h>
//#include <errno.h>
//#include <cstring>
//
// namespace tk {
// namespace asio {
// class Dispatcher;
//
// namespace stream {
//
// template <typename T>
// class Sender {
// public:
//  typedef void (*SendCallback)(T*, void*, Status, Error, long);
//
//  Sender(Dispatcher* dispatcher) : _dispatcher(dispatcher) {}
//
//  static void SenderHandler(void* data) {
//    Sender* sender = (Sender*)data;
//
//#ifdef __APPLE__
//    sender->_status = send(sender->_fd, sender->_data, sender->_data_size, 0);
//#else
//    sender->_status = send(sender->_fd, sender->_data, sender->_data_size, MSG_NOSIGNAL);
//#endif
//
//    /// Could not send data.
//    if (sender->_status == -1) {
//      // Resource temporarily unavailable.
//      if (errno == EAGAIN) {
//#ifdef __TK_CONSOLE__
//        console::Error(TK_TRACER, "Resource is still unavailable (THIS SHOULD NEVER HAPPEND).");
//        console::FError<console::FLogType::ASIO>(TK_TRACER,
//                                                 "Resource is still unavailable (THIS SHOULD NEVER
//                                                 HAPPEND).");
//#endif  // __TK_CONSOLE__
//        return;
//      } else if (errno == EPIPE) {  // Broken pipe.
//#ifdef __TK_CONSOLE__
//        console::FError<console::FLogType::ASIO>(TK_TRACER, "Broken pipe.");
//#endif  // __TK_CONSOLE__
//        sender->CallUserCallback(Status::kBad, Error::kBrokenPipe);
//        return;
//      }
//
//// Unhandled error (for now).
//#ifdef __TK_CONSOLE__
//      console::Error(TK_TRACER, "Could not send to socket :", std::strerror(errno));
//      console::FError<console::FLogType::ASIO>(TK_TRACER, "Could not send to socket :",
//      std::strerror(errno));
//#endif  // __TK_CONSOLE__
//      sender->CallUserCallback(Status::kBad, Error::kUnknown);
//      return;
//    }
//
//    if (sender->_status == 0) {
///// @todo Don't know exactly what to do there.
//#ifdef __TK_CONSOLE__
//      console::Warning(TK_TRACER, "Could not send to socket again ( write size :", 0, ").",
//      std::strerror(errno));
//      console::FWarning<console::FLogType::ASIO>(TK_TRACER, "Could not send to socket again ( write size :",
//      0, ").",
//                                                 std::strerror(errno));
//
//      /// @todo Add timeout ???
//      console::Error(TK_TRACER, "@@@@TODO MAYBE ADD TIMEOUT ??? Socket got disconnected ?????");
//#endif  // __TK_CONSOLE__
//      sender->CallUserCallback(Status::kBad, Error::kReaderWasClosed);
//      return;
//    }
//
//    sender->CallUserCallback(Status::kGood, Error::kNone);
//  }
//
//  long AsyncWrite(T* owner, int fd, void* const data, std::size_t data_size, SendCallback callback, void*
//  user_data) {
//    _fd = fd;
//    _owner = owner;
//    _data_size = data_size;
//    _data = data;
//    _callback = callback;
//    _user_data = user_data;
//
//    if (!_callback) {
//#ifdef __TK_CONSOLE__
//      console::Warning(TK_TRACER, "Sender callback wasn't set.");
//      console::FWarning<console::FLogType::ASIO>(TK_TRACER, "Sender callback wasn't set.");
//#endif  // __TK_CONSOLE__
//      return -1;
//    }
//
//    // Invalid file descriptor.
//    if (_fd == -1) {
//#ifdef __TK_CONSOLE__
//      console::Error(TK_TRACER, "Try to send on empty socket.");
//      console::FError<console::FLogType::ASIO>(TK_TRACER, "Try to send on empty socket.");
//#endif  // __TK_CONSOLE__
//      _status = -1;
//      return CallUserCallback(Status::kBad, Error::kInvalidFileDescriptor);
//    }
//
//#ifdef __APPLE__
//    _status = send(_fd, data, data_size, 0);
//#else
//    _status = send(_fd, data, data_size, MSG_NOSIGNAL);
//#endif
//
//    // If could not send data.
//    if (_status == -1) {
//      // Resource temporarily unavailable (which is normal since socket is on non-blocking mode).
//      if (errno == EAGAIN) {
//        // Add output handler to io dispatcher.
//        _dispatcher->AddHandler(_fd, &Sender::SenderHandler, this, Dispatcher::HandleType::Output);
//        return _status;
//      } else if (errno == EPIPE) {  // Broken pipe.
//#ifdef __TK_CONSOLE__
//        console::FError<console::FLogType::ASIO>(TK_TRACER, "Broken pipe.");
//#endif  // __TK_CONSOLE__
//        return CallUserCallback(Status::kBad, Error::kBrokenPipe);
//      }
//
//// Unhandled error (for now).
//#ifdef __TK_CONSOLE__
//      console::Error(TK_TRACER, "Write :", std::strerror(errno));
//      console::FError<console::FLogType::ASIO>(TK_TRACER, "Write :", std::strerror(errno));
//#endif  // __TK_CONSOLE__
//      return CallUserCallback(Status::kBad, Error::kUnknown);
//    }
//
//    if (_status == 0) {
///// @todo Don't know exactly what to do there.
///// @todo Add timeout ???
//#ifdef __TK_CONSOLE__
//      console::Error(TK_TRACER, "@@@@TODO MAYBE ADD TIMEOUT ??? Socket got disconnected ?????");
//      console::FError<console::FLogType::ASIO>(TK_TRACER,
//                                               "@@@@TODO MAYBE ADD TIMEOUT ??? Socket got disconnected
//                                               ?????");
//#endif  // __TK_CONSOLE__
//      return CallUserCallback(Status::kBad, Error::kReaderWasClosed);
//    }
//
//    return CallUserCallback(Status::kGood, Error::kNone);
//  }
//
// private:
//  long _status = 0;
//  int _fd = -1;
//  std::size_t _data_size = 0;
//  SendCallback _callback = nullptr;
//  void* _data = nullptr;
//  void* _user_data = nullptr;
//  T* _owner = nullptr;
//  Dispatcher* _dispatcher;
//
//  long CallUserCallback(Status status, Error error) {
//    _callback(_owner, _user_data, status, error, _status);
//    return _status;
//  }
//};
//
//}  // stream.
//}  // asio.
//}  // tk.