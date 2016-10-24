#pragma once

#include "dispatcher.h"
#include "stream/reader.h"
//#include "tk/asio/stream/writer.hh"

 namespace fst {
 namespace io {
// class Dispatcher;

 class file {
 public:
  file(dispatcher* io_dispatcher) : _fd(-1), _io_dispatcher(io_dispatcher), _reader(io_dispatcher) {
	  
  }

  ~file() {
	  if (_fd != -1) {
    	close(_fd);
    	_io_dispatcher->remove_handler(_fd);
	  }
  }

  inline int get_fd() const { return _fd; }

  inline bool is_valid() const { return _is_valid == 1; }

  long async_read(void* data, std::size_t data_size, stream::Reader<file>::ReadCallback callback,
                 void* user_data = nullptr) {
	  return _reader.AsyncRead(this, _fd, data, data_size, callback, user_data);
  }

//  long AsyncWrite(void* data, std::size_t data_size, stream::Writer<File>::WriteCallback callback,
//                  void* user_data = nullptr);

//  void Close();

//  void CloseWrite();
  
  int init(const char* filename) {
	  _fd = open(filename, O_RDWR);
	  
	  if (_fd == -1) {
//#ifdef __TK_CONSOLE__
//    tk::console::Error("Could not open file descriptor :", _fd);
//#endif  // __TK_CONSOLE__
    return -1;
	  }
	  
	  int flags = fcntl(_fd, F_GETFL, 0);
	  if (flags < 0) {
		  return 3;
	  }
	  flags = (flags & ~O_NONBLOCK);
	  
	  if (fcntl(_fd, F_SETFL, flags) != 0) {
		  errprint("Can't set file to non-blocking.");
		  return 4;
	  }
	  return 0;
	  }

 private:
  int _fd;
  int _is_valid;
  dispatcher* _io_dispatcher;
  stream::Reader<file> _reader;
//  stream::Writer<File> _writer;

//  int Init(const char* filename);
};

}  // io.
}  // fst.
