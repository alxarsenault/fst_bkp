#pragma once

#include <errno.h>
#include <fcntl.h>
#include <poll.h> // pollfd.

#include "../print.h"
#include "../time.h"
#include "../unordered_vector.h"

namespace fst {
namespace io {

	//	template <std::size_t N = 16>
	class dispatcher {
	private:
		struct handler_data {
			int fd; ///< File descriptor.
			void (*handler_callback)(void*); ///< Handler callback function.
			void* data; ///< Data passed to handler_callback argument.
		};

		short HandleTypeToPollFlag[2] = { POLLIN | POLLPRI, POLLOUT | POLLWRBAND };

		static void dummy_pipe_handler(void* data)
		{
			// TK_CONSOLE(console::Error("Receive dummy pipe data (THIS SHOULD NEVER
			// HAPPEND");)
			// TK_CONSOLE(console::Error("This could be use to send command to io
			// dispatcher in the futur.");)
		}

	public:
		enum handle_type { input, output };

		enum class status { good };

		dispatcher()
		{
			// Init dummy pipe.
			int err = pipe(_dummy_pipe_fd);

			if (err == -1) {
				// TK_CONSOLE(console::Error("Could not create dummy pipe."));
				return;
			}

			//						 if (!SetSocketBlockingEnabled(_dummy_pipe_fd[0], true))
			//{
			//							 TK_CONSOLE(console::Error("Could not set dummy pipe
			// to
			// non-blocking."));
			//							 return;
			//						 }

			int flags = fcntl(_dummy_pipe_fd[0], F_GETFL, 0);
			if (flags < 0) {
				return;
			}
			flags = (flags & ~O_NONBLOCK);
			;
			if (fcntl(_dummy_pipe_fd[0], F_SETFL, flags) != 0) {
				errprint("Can't set local::Socket to non-blocking.");
				return;
			}

			_handler_list[handle_type::input].emplace_back(
				handler_data{ _dummy_pipe_fd[0], dummy_pipe_handler, nullptr });
			_poll_list.emplace_back(pollfd{ _dummy_pipe_fd[0], HandleTypeToPollFlag[handle_type::input], 0 });
		}

		~dispatcher()
		{
			if (_dummy_pipe_fd[0] != -1) {
				close(_dummy_pipe_fd[0]);
			}

			if (_dummy_pipe_fd[1] != -1) {
				close(_dummy_pipe_fd[1]);
			}
		}

		int run(int time_out_ms = -1, bool call_timeout_before = false)
		{
			_is_running = 1;

			// Start reading on dummy pipe.
			read(_dummy_pipe_fd[0], _dummy_pipe_fd, _dummy_pipe_read_buffer);

			if (call_timeout_before && _time_out_callback) {
				_time_out_callback(_time_out_data);
			}

			int current_time_out_ms = time_out_ms;
			//////////////////tk::time::Clock begin_time;
			fst::time begin_time;

			while (_is_running) {
				/// @todo Remove this.
				if (!_poll_list.size()) {
					fst::warnprint("POLL SIZE IS EMPTY !!!");
				}

				int retval = poll(_poll_list.data(), (nfds_t)_poll_list.size(), current_time_out_ms);

				// If poll returned error.
				if (retval < 0) {
					// Interrupted system call.
					if (errno == EINTR) {
						continue;
					} else { // Stop on any other error.
						fst::errprint("Can't poll :", std::strerror(errno));
						return retval;
					}
				}

				// Timeout.
				if (retval == 0) {
					if (time_out_ms != -1 && _time_out_callback) {
						_time_out_callback(_time_out_data);

						// Reinit begin time.
						current_time_out_ms = time_out_ms;
						/////////////begin_time = tk::time::Clock();
					}
					continue;
				}

				int l_size = _poll_list.size();

				// Loop through poll list.
				for (int i = 0; i < l_size; i++) {

					if (_poll_list[i].revents == 0) {
						// TK_CONSOLE(console::FLog<console::FLogType::ASIO>(TK_TRACER,
						// "revents == 0");)
						continue;
					}

					//#define	POLLIN		0x0001		/* any readable data
					// available */
					//#define	POLLPRI		0x0002		/* OOB/Urgent
					// readable data */
					//#define	POLLOUT		0x0004		/* filedescriptor is
					// writeable */
					//#define	POLLRDNORM	0x0040		/* non-OOB/URG data
					// available */
					//#define	POLLWRNORM	POLLOUT		/* no write type
					// differentiation */
					//#define	POLLRDBAND	0x0080		/* OOB/Urgent readable
					// data */
					//#define	POLLWRBAND	0x0100		/* OOB/Urgent data can
					// be written */

					/*
					 * FreeBSD extensions: polling on a regular file might return one
					 * of these events (currently only supported on local filesystems).
					 */
					//#define	POLLEXTEND	0x0200		/* file may have been
					// extended */
					//#define	POLLATTRIB	0x0400		/* file attributes may
					// have changed */
					//#define	POLLNLINK	0x0800		/* (un)link/rename may
					// have happened */
					//#define	POLLWRITE	0x1000		/* file's contents may
					// have changed */

					/*
					 * These events are set if they occur regardless of whether they were
					 * requested.
					 */
					//#define	POLLERR		0x0008		/* some poll error
					// occurred */
					//#define	POLLHUP		0x0010		/* file descriptor
					// was "hung up" */
					//#define	POLLNVAL	0x0020		/* requested events
					//"invalid" */

					if (_poll_list[i].revents == POLLHUP) {
						//						TK_CONSOLE(console::FLog<console::FLogType::ASIO>(
						//							TK_TRACER, "POLLHUP : file descriptor
						// was hung up.");)

						// Need to close this fd.
						continue;
					}

					// Handle output.
					if ((_poll_list[i].revents & HandleTypeToPollFlag[handle_type::output])) {
						//						TK_CONSOLE(console::FLog<console::FLogType::ASIO>(
						//							TK_TRACER, "POLLOUT : file descriptor is
						// writeable.");)

						for (const auto& handler : _handler_list[handle_type::output]) {
							if (handler.fd == _poll_list[i].fd) {
								handler.handler_callback(handler.data);
								break;
							}
						}
						continue;
					}

					// Handle input.
					if ((_poll_list[i].revents & HandleTypeToPollFlag[handle_type::input])) {
						//						TK_CONSOLE(console::FLog<console::FLogType::ASIO>(
						//							TK_TRACER, "POLLIN : any readable data
						// available.");)

						// Find handler.
						for (const auto& handler : _handler_list[handle_type::input]) {
							if (handler.fd == _poll_list[i].fd) {
								handler.handler_callback(handler.data);
								break;
							}
						}

						continue;
					}

					// Could not handle revents, it's an unexpected result (for now).
					fst::errprint(ptrace, "revents :", _poll_list[i].revents);
					//					TK_CONSOLE(console::Error(TK_TRACER,
					//"revents :",
					//_poll_list[i].revents);)
					//					TK_CONSOLE(console::FError<console::FLogType::ASIO>(
					//						TK_TRACER, "revents :",
					//_poll_list[i].revents);)
					_is_running = 0;
					break;
				}

				//				TK_CONSOLE(console::FLog<console::FLogType::ASIO>(
				//					TK_TRACER, "Delete before size
				//:", _poll_list.GetSize());)

				_poll_list.erase_if([](const pollfd& pfd) { return pfd.fd == -1; });
				_handler_list[handle_type::input].erase_if(
					[](const handler_data& hd) { return hd.fd == -1; });
				_handler_list[handle_type::output].erase_if(
					[](const handler_data& hd) { return hd.fd == -1; });

				//				TK_CONSOLE(console::FLog<console::FLogType::ASIO>(
				//					TK_TRACER, "Delete after size
				//:", _poll_list.GetSize());)

				if (time_out_ms != -1) {
					//					const long diff_ms = (tk::time::Clock()
					//- begin_time).GetMs();
					const long diff_ms = (fst::time() - begin_time).ms();

					if (diff_ms < time_out_ms) {
						current_time_out_ms = time_out_ms - diff_ms;
					} else {
						// Call timeout callback.
						_time_out_callback(_time_out_data);
						current_time_out_ms = time_out_ms;
						//						begin_time =
						// tk::time::Clock();
						begin_time = fst::time();
					}
				}
			}

			return 0;
		}

		void stop();

		void add_timeout_handler(void (*time_out_callback)(void*), void* data)
		{
			_time_out_callback = time_out_callback;
			_time_out_data = data;
		}

		int add_handler(int fd, void (*handler_callback)(void*), void* data, handle_type type)
		{
			bool found = false;
			_handler_list[type].operation_first_if([fd](const handler_data& hd) { return hd.fd == fd; },
				[&found, handler_callback, data](handler_data& hd) {
					hd.handler_callback = handler_callback;
					hd.data = data;
					found = true;
				});

			if (!found) {
				_handler_list[type].emplace_back(handler_data{ fd, handler_callback, data });
				_poll_list.emplace_back(pollfd{ fd, HandleTypeToPollFlag[type], 0 });
			}

			// TK_CONSOLE(console::FLog<console::FLogType::ASIO>(TK_TRACER, "Add handler
			// to socket :", fd);)
			return 0;
		}

		void remove_handler(int fd)
		{
		}

		void remove_handler(int fd, handle_type type);

	private:
		fst::unordered_vector<pollfd, 16, false> _poll_list;
		fst::unordered_vector<handler_data, 16, false> _handler_list[2];
		//  tk::container::UnorderedArray<32, pollfd> _poll_list;
		//  tk::container::UnorderedArray<32, handler_data> _handler_list[2];

		int _is_running;

		void (*_time_out_callback)(void*) = nullptr; ///< Timeout callback function.
		void* _time_out_data = nullptr; ///< Data passed to _time_out_callback argument.

		// Dummy pipe.
		int _dummy_pipe_fd[2];
		char _dummy_pipe_read_buffer;
	};
} // io.
} // fst.
