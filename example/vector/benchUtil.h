#pragma once

#include <iomanip>
#include <iostream>
#include <string>
#include <chrono>

/*
	escape(void*) and clobber() are from Chandler Carruth. More info:
	https://www.youtube.com/watch?v=nXaxk27zwlk
	Ty Chandler!
*/

namespace Bench {
	std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
	std::chrono::duration<double> elapsedTime;

	void title(const std::string& title) {
		std::cout << std::endl << std::string(title.size(), '#') << std::endl;
		std::cout << title << std::endl;
		std::cout << std::string(title.size(), '#') << std::endl << std::endl;
	}

	void start(const std::string& message = "") {
		if (!message.empty()) {
			std::cout << std::endl << message << std::endl;
			std::cout << std::string(message.size(), '-') << std::endl;
		}
		startTime = std::chrono::system_clock::now();
	}

	void end(const std::string& message = "") {
		endTime = std::chrono::system_clock::now();
		elapsedTime = endTime - startTime;

		std::cout << std::setw(70) << std::left << message
				<< std::setiosflags(std::ios::fixed)
				<< elapsedTime.count() << "s" << std::endl;
	}

	/*
	This deactivates compiler optimizations for the passed pointer.
	It indicates that the pointer memory "could" have been modified.

	Usage: Pass the pointer to an allocated object you want to benchmark.
	*/
	static void escape(void *p) {
#ifdef _MSC_VER
		_WriteBarrier();
#else
		asm volatile("" : : "g"(p) : "memory");
#endif
	}

	/*
	This method deactivates compiler optimizations by indicating all memory
	could have been modified.

	Usage: Use after a call, to make sure the compiler doesn't remove the call.
	*/
	static void clobber() {
#ifdef _MSC_VER
		_ReadBarrier();
#else
		asm volatile("" : : : "memory");
#endif
	}
};