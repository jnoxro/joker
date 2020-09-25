//============================================================================
// Name        : timer.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : basic timer to make code cleaner
//============================================================================

#include <string>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <chrono>


#include "timer.h"

using namespace std;
using namespace std::chrono;

timer::timer()
{

}

void timer::start(string s, int v)
{
	verbosity = v;
	id = s;
	if (verbosity > 0)
	{
		starttime = steady_clock::now();
	}
}

void timer::end()
{
	if (verbosity == 1 || verbosity == 3)
	{
		endtime = steady_clock::now();
		auto dur = duration_cast<microseconds>(endtime-starttime).count();
		cout << "[Joker] Timer " << id << ": " << dur << " microsecs" << endl;
	}
}
