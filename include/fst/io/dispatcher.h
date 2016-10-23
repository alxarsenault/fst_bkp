//#pragma once
//
///**
// * @file   dispatcher.hh
// * @author Alexandre Arsenault (aarsenault@erftcomposites.com)
// * @date   13/09/2016
// * @brief  Dispatcher class.
// */
//
//#include <poll.h>  // pollfd.
//#include "tk/container/unordered_array.hh"
//
//namespace tk {
//namespace asio {
///**
// * @addtogroup tk
// * @{
// * @addtogroup asio
// * @{
// */
//
///**
// * @brief Waits for one of a set of file descriptors to become ready to perform asynchronous I/O operation.
// */
//class Dispatcher {
// public:
//  /**
//   * @enum HandleType
//   * @brief List of possible handle type.
//   */
//  enum HandleType { Input, Output };
//
//  enum class Status { kGood };
//
//  /**
//   * @brief Default constructor.
//   * @details Doesn't do anything.
//   */
//  Dispatcher();
//
//  ~Dispatcher();
//
//  /**
//   * @brief Begin blocking polling.
//   * @details This function will block current thread and call appropriate handler when a file descriptor is ready to
//   *          perform I/O operation. This will loop forever unless it receives a poll error.
//   * @param time_out_ms Set time in milliseconds between each call to timeout handler. Use AddTimeOutHandler to
//   *        set the timeout callback. If timeout is set to -1, poll will wait forever untill a file descriptor is ready.
//   * @return Zero on success or -1 on fail.
//   */
//  int Run(int time_out_ms = -1, bool call_timeout_before = false);
//
//  /**
//   * @brief Stop main loop.
//   */
//  void Stop();
//
//  /**
//   * @brief Add timeout callback.
//   * @details This is intended to be use as a program loop timer. It will never be call if Run(time_out_ms) is -1.
//   * @param time_out_callback Callback function called when time_out_ms time from Run argument as been reach.
//   * @param data Data passed as argument to the time_out_callback.
//   */
//  void AddTimeOutHandler(void (*time_out_callback)(void*), void* data);
//
//  /**
//   * @brief Add file descriptor handler to the polling list.
//   * @param fd File descriptor.
//   * @param handler_callback Callback function called when file descriptor is ready to perform I/O operation given by
//   *        type HandleType.
//   * @param data Data passed as argument to the handler_callback.
//   * @param type Type of handler (input or output).
//   * @return Always returns zero for now.
//   */
//  int AddHandler(int fd, void (*handler_callback)(void*), void* data, HandleType type);
//
//  /**
//   * @brief Remove file descriptor handler from polling list.
//   * @details This will remove all handler of all types associated with the given file descriptor.
//   * @param fd File descriptor to be remove from polling list.
//   */
//  void RemoveHandler(int fd);
//
//  /**
//   * @brief Remove file descriptor handler from HandleType type polling list.
//   * @details This will remove all handler of type HandleType associated with the given file descriptor.
//   * @param fd File descriptor to be remove from HandleType polling list.
//   */
//  void RemoveHandler(int fd, HandleType type);
//
// private:
//  /**
//   * @brief Handler informations kept in _handler_list.
//   * @details This will be call when current file descriptor is ready for I/O operation.
//   */
//  struct HandlerData {
//    int fd;                           ///< File descriptor.
//    void (*handler_callback)(void*);  ///< Handler callback function.
//    void* data;                       ///< Data passed to handler_callback argument.
//  };
//
//  /// List of all file descriptor with given HandleType association passed to AddHandler.
//  tk::container::UnorderedArray<32, pollfd> _poll_list;
//
//  /// List of handler data associated with each pollfd file descriptors in _poll_list.
//  /// [0] = HandleType::Input.
//  /// [1] = HandleType::Output.
//  tk::container::UnorderedArray<32, HandlerData> _handler_list[2];
//
//  int _is_running;
//
//  void (*_time_out_callback)(void*) = nullptr;  ///< Timeout callback function.
//  void* _time_out_data = nullptr;               ///< Data passed to _time_out_callback argument.
//
//  // Dummy pipe.
//  int _dummy_pipe_fd[2];
//  char _dummy_pipe_read_buffer;
//};
//
///// @}
///// @}
//}  // asio.
//}  // tk.
