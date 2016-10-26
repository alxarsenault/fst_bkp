#include <fst/io/dispatcher.h>
#include <fst/io/tcp/acceptor.h>

#include <fst/print.h>
#include <fst/vector.h>

char buffer[1024];

int main()
{
	fst::vector<fst::io::tcp::socket, 16> sessions_vec;
	fst::io::dispatcher io_dispatcher;
	fst::io::tcp::acceptor tcp_acceptor(&io_dispatcher, 8012);

	tcp_acceptor.async_accept(
		[](fst::io::tcp::socket&& sock, const fst::io::ip::address& ip_addr, void* data) {
			fst::print("got connection from :", ip_addr.data());
			auto* sessions_vec = static_cast<fst::vector<fst::io::tcp::socket, 16>*>(data);

			sock.async_read((void*)buffer, 1024, [](fst::io::tcp::socket*, void* data _FST_UNUSED,
													 fst::io::status status, long n_bytes _FST_UNUSED) {
				if (status.state == fst::io::state::bad) {
					return;
				}

				fst::print(buffer);
			});

			sessions_vec->emplace_back(std::move(sock));
		},
		&sessions_vec);

	io_dispatcher.add_timeout_handler([](void* data _FST_UNUSED) { fst::tprint("timeout"); }, nullptr);
	io_dispatcher.run(2000);
	return 0;
}
