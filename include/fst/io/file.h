//#pragma once
//
//#include "tk/asio/stream/reader.hh"
//#include "tk/asio/stream/writer.hh"
//
//namespace tk {
//namespace asio {
//class Dispatcher;
//
//class File {
// public:
//  File(Dispatcher* dispatcher, const char* filename);
//
//  ~File();
//
//  inline int GetFd() const { return _fd; }
//
//  inline bool IsValid() const { return _is_valid == 1; }
//
//  long AsyncRead(void* data, std::size_t data_size, stream::Reader<File>::ReadCallback callback,
//                 void* user_data = nullptr);
//
//  long AsyncWrite(void* data, std::size_t data_size, stream::Writer<File>::WriteCallback callback,
//                  void* user_data = nullptr);
//
//  void Close();
//
//  void CloseWrite();
//
// private:
//  int _fd;
//  int _is_valid;
//  Dispatcher* _dispatcher;
//  stream::Reader<File> _reader;
//  stream::Writer<File> _writer;
//
//  int Init(const char* filename);
//};
//
//}  // asio.
//}  // tk.
