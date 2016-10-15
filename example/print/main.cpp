#include <fst/print.h>
#include <string>
#include <vector>

class MyClass {
public:
	MyClass(int a, int b)
		: _a(a)
		, _b(b)
	{
	}

	operator fst::streamer()
	{
		return fst::stream("a :", _a, "-->", "b :", _b);
	}

private:
	int _a, _b;
};

class YourClass {
public:
	YourClass(int a, int b)
		: _a(a)
		, _b(b)
	{
	}

	// No fst::Streamer implemented.

private:
	int _a, _b;
};

int main() {
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
			   "Michelle", fst::color::bold_blue, ftime, fst::color::bold_green,
			   ftrace, fst::color::reset, "McMurphy");
	
	fst::print(fst::color::bold_cyan, "[DEBUG]", fst::color::reset);
	fst::print(std::string("std::string"));
	
	MyClass mc(1, 2);
	fst::print("MyClass :", mc);
	fst::print("MyClass :", MyClass(3, 4));
	fst::print("MyClass :", (fst::streamer)mc);
	fst::print("YourClass :", YourClass(8, 9));
	
	/// @todo Check this error.
	fst::streamer ss = fst::stream("Alex", 1989);
	fst::print("print stream :", ss);
	
//	fst::print(fst::stream("John"));
	
	std::vector<int> vec = {0, 1, 2, 3, 4};
	std::vector<double> vecf = {0.0, 1.0, 2.0, 3.0, 4.0};
	
	fst::print("vec :", vec);
	fst::print("vec :", fst::iterable(vec.data(), vec.size()));
	fst::print("vec :", fst::iterable(&vec[0], &vec[vec.size()]));
	fst::print("vec :", fst::iterable(&vecf[0], &vecf[vecf.size()]));
	
	int bomb[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
	fst::print("array :", fst::iterable(bomb, sizeof(bomb) / sizeof(int)));
	return 0;
}
