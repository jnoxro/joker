//============================================================================
// Name        : ocr.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : OCR class, uses trained model to predict letter
//============================================================================

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <string>
#include <atomic>
#include <chrono>
#include <thread>
#include <tuple>

#include "ocr.h"
#include "imgvect.h"
#include "model.h"
#include "timer.h"

using namespace std;
using namespace std::chrono;

ocr::ocr(string modelname, int threads, int verbose)
{
	verbosity = verbose;

	timer1.start("ocr: model load", verbosity);
	mymodel.load(modelname);
	timer1.end();

	initthreadpool(threads);
}

void ocr::initthreadpool(int threads)
{
	if (threads != 1)
	{
		extrathreads = true;
		if (threads == 0) //zero means auto
		{
			workercount = thread::hardware_concurrency();
		}
		else
		{
			workercount = threads;
		}
		if (verbosity == 1 || verbosity == 3)
		{
			cout << "[Joker] Ocr: initthreadpool: worker count " << workercount << endl;
		}
		for (int tc = 0; tc < workercount; tc++)
		{
			threadpool.push_back(thread(&ocr::worker, this, tc, workercount)); //tc acts as thread id
		}

	}
}

tuple<string, long> ocr::solve(vector<long> target)
{
	image.imgcontainer = target;
	addjob();
	return make_tuple(finalresult, finalscore);
}

tuple<string, long> ocr::solve(string filepath)
{
	if(image.read(filepath, 125) == 0)
	{
		cerr << "[Joker] Error: ocr: solve: image open error" << endl;
		finalresult = "";
		finalscore = -500000;
	}
	else
	{
		addjob();
	}

	return make_tuple(finalresult, finalscore);
}
