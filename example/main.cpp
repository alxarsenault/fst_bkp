#include <fst/print>

int main(int argc, const char* argv[])
{
	fst::print("Bang", 293);
	fst::print<10>("Bang", 293);

	fst::cprint(fst::color::blue, "blue");
	fst::cprint(fst::color::red, "red");
	fst::cprint(fst::color::yellow, "yellow");
	fst::cprint(fst::color::cyan, "cyan");
	fst::cprint(fst::color::magenta, "magenta");
	fst::cprint(fst::color::green, "green");
	
	fst::tprint("Log ...");

	fst::errprint("Banana");
	fst::errprint("George King");
	fst::errprint("Banana");

	fst::warnprint("Bang");
	fst::warnprint("Bang");
	fst::warnprint(fst::color::black, "black warning");
	fst::errprint("bb", 392, 3234);
	fst::errprint(fst::color::black, "black error");

	fst::print(fst::color::bold_black, "Bang", 293, 323);
	fst::print("Still bold black", fst::color::reset);
	fst::cprint(fst::color::blue, "blue");
	fst::print("Not blue");

	fst::print(ftrace, "peter");
	fst::print(ptrace, "peter");
	fst::print("peter", ftrace, "john");

	fst::print(fst::color::bold_cyan, "[DEBUG]", fst::color::bold_magenta,
		"Michelle", fst::color::bold_blue, ftime, fst::color::bold_green, ftrace,
		fst::color::reset, "McMurphy");

	fst::print(fst::color::bold_cyan, "[DEBUG]", fst::color::reset);
	return 0;
}
