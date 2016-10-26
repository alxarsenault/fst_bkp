#include <fst/io/dispatcher.h>
#include <fst/io/file.h>
#include <fst/io/tcp/socket.h>

#include <fst/print.h>
#include <fst/vector.h>

int main()
{
	fst::io::dispatcher io_dispatcher;
	fst::io::tcp::socket sock(&io_dispatcher);
	sock.init("127.0.0.1", 8012);

	if (!sock.is_valid()) {
		fst::errprint("Can't connect socket.");
		return -1;
	}

	const char* buffer = "Alexandre";
	sock.async_write((void*)buffer, 9, [](fst::io::tcp::socket* sock, void* usr_data _FST_UNUSED,
										   fst::io::status status, long n_bytes _FST_UNUSED) {
		if (status.state == fst::io::state::bad) {
			fst::print("Error sendind data.");
			sock->get_dispatcher()->stop();
			return;
		}

		sock->get_dispatcher()->stop();
	});

	io_dispatcher.add_timeout_handler([](void* data _FST_UNUSED) { fst::tprint("timeout"); });
	io_dispatcher.run(2000);
	return 0;
}
