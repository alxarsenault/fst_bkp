//#pragma once
//
//#include "tk/asio/reader.hh"
//
// namespace tk {
// namespace asio {
// class Dispatcher;
//
// class Serial {
// public:
//  Serial(Dispatcher* dispatcher, const char* filename, int baud_rate);
//
//  ~Serial();
//
//  inline int GetFd() const { return _fd; }
//
//  inline bool IsValid() const { return _is_valid == 1; }
//
//  long AsyncRead(void* data, std::size_t data_size,
//  Reader<Serial>::ReadCallback callback, void* user_data =
//  nullptr);
//
//  void Close();
//
// private:
//  int _fd;
////  int _port;
//  int _is_valid;
//  Dispatcher* _dispatcher;
//  Reader<Serial> _reader;
//
//    int Init(const char* filename, int baud_rate);
//};
//
//}  // asio.
//}  // tk.