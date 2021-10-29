#pragma once

#include <chrono>
#include <iostream>

class Timer {
public:
#ifdef WIN32
	std::chrono::time_point<std::chrono::steady_clock> start, end;
#endif // win32

#ifdef __linux__ 
	std::chrono::_V2::system_clock::time_point start, end;
#endif // linux

	std::chrono::duration<float> duration;

	Timer() {
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count() * 1000.0f;
		std::cout <<"Job took " << ms << "ms \n";
	}
	

private:

};

