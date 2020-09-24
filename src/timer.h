//============================================================================
// Name        : timer.h
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Header file for timer.cpp
//============================================================================

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <string>

class timer
{
	private:
		std::chrono::time_point<std::chrono::steady_clock> starttime = std::chrono::steady_clock::now();
		std::chrono::time_point<std::chrono::steady_clock> endtime = std::chrono::steady_clock::now();

		std::string id = "";
		int verbosity = 0;

	public:
		timer();

		void start(std::string s, int v);
		void end();

};


#endif /* TIMER_H_ */
