#include <fst/print.h>
#include <fst/io/dispatcher.h>
#include <fst/io/local_socket.h>
#include <fst/io/file.h>

char buffer[1024];
char buffer2[1024];
char buffer3[1024];
int main()
{
	fst::io::dispatcher io_dispatcher;
//	fst::io::local::socket local_socket(&io_dispatcher);
//	local_socket.init("./dsa");


	fst::io::file f(&io_dispatcher);
	fst::io::file f2(&io_dispatcher);
	fst::io::file f3(&io_dispatcher);
	f.init("/Users/alexarse/Desktop/server.py");
	f2.init("/Users/alexarse/Desktop/server.py");
	f3.init("/Users/alexarse/Desktop/server.py");
		f.async_read(
			buffer, 1024, [](fst::io::file*, void*, fst::io::status status, fst::io::error error, long) {
				if (status == fst::io::status::bad) {
					fst::print("bad chunky", (int)error);
					return;
				}
				//fst::print((const char*)buffer);
			});
	
	
	
	
	f2.async_read(
				 buffer2, 1024, [](fst::io::file*, void*, fst::io::status status, fst::io::error error, long) {
					 if (status == fst::io::status::bad) {
						 fst::print("bad chunky", (int)error);
						 return;
					 }
					 //fst::print((const char*)buffer2);
				 });
	
	f3.async_read(
				  buffer3, 1024, [](fst::io::file*, void*, fst::io::status status, fst::io::error error, long) {
					  if (status == fst::io::status::bad) {
						  fst::print("bad chunky", (int)error);
						  return;
					  }
					  //fst::print((const char*)buffer2);
				  });
	
//	local_socket.async_read(
//		buffer, 1024, [](fst::io::local::socket*, void*, fst::io::status status, fst::io::error error, long) {
//			if (status == fst::io::status::bad) {
//				fst::print("bad chunky", (int)error);
//				return;
//			}
//			fst::print("chunky");
//		});

	io_dispatcher.add_timeout_handler([](void* data){
		fst::tprint("timeout");
	}, nullptr);

	io_dispatcher.run(2000);
	return 0;
}
