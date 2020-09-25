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

	terminate = false;
	timer1.start("ocr: initthreadpool", verbosity);
	initthreadpool(threads);
	timer1.end();
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
		for (int tc = 0; tc < workercount; tc++)
		{
			threadpool.push_back(thread(&ocr::worker, this, tc, workercount)); //tc acts as thread id
		}
		while (threadready.load() < workercount)
		{
			continue;
		}
	}
	if (verbosity == 2 || verbosity == 3)
	{
		cout << "[Joker] ocr: initthreadpool: worker count " << workercount << "\n";
	}
}

tuple<string, long> ocr::solve(vector<int> target)
{
	timer1.start("ocr: solve: image read", verbosity);
	if ((long)target.size() == mymodel.pamodel.height * mymodel.pamodel.width)
	{
		timer1.end();
		image.imgcontainer = target;
		timer1.start("ocr: solve: job", verbosity);
		job();
		timer1.end();
	}
	else
	{
		timer1.end();
		cerr << "[Joker] Error: ocr: solve: size mismatch\n";
		exit(EXIT_FAILURE);
	}
	return make_tuple(pa.finalresult, (long)pa.finalscore.load());
}

tuple<string, long> ocr::solve(string filepath)
{
	timer1.start("ocr: solve: image read", verbosity);
	if(image.read(filepath, 125) == 1)
	{
		timer1.end();
		timer1.start("ocr: solve: job", verbosity);
		job();
		timer1.end();
	}
	else
	{
		timer1.end();
		cerr << "[Joker] Error: ocr: solve: image open error\n";
		exit(EXIT_FAILURE);
	}

	return make_tuple(pa.finalresult, (long)pa.finalscore.load());
}

void ocr::job()
{
	if(mymodel.methodology == "pixelaverage")
	{
		pa.assignmentqueue = (int)mymodel.pamodel.map.size();

		if (workercount == 1)
		{
			worker(1,1); //if not multithreading, call a single worker function
		}

		while (pa.assignmentsfinished.load() < (int)mymodel.pamodel.map.size()) //wait till all assignments finished
		{
			continue;
		}

		cout << flush;
	}
}

void ocr::worker(int id, int numworkers)
{
	if (verbosity == 2 || verbosity == 3)
	{
		coutmtx.lock();
		cout << "[Joker] ocr: worker " << id << " started\n";
		coutmtx.unlock();
	}
	threadready++;

	int assignmentsize = (int)mymodel.pamodel.map.size() / numworkers;
	do
	{
		if (mymodel.methodology == "pixelaverage")
		{

			int startpoint = 0;
			pa.assignmentqueuemtx.lock();
			if (pa.assignmentqueue.load() > 0)
			{
				if (pa.assignmentqueue >= assignmentsize)
				{
					pa.assignmentqueue -= assignmentsize;
				}
				else
				{
					assignmentsize = pa.assignmentqueue;
					pa.assignmentqueue = 0;
				}
				startpoint = pa.assignmentqueue;
				pa.assignmentqueuemtx.unlock();

				auto [result, score] = solvepixelaverage(startpoint, assignmentsize, id);

				//pa.assignmentfinishedmtx.lock();
				if (verbosity > 1)
				{
					coutmtx.lock();
					cout << id << " returned info: " << result << " " << score << endl;
					coutmtx.unlock();
				}
				if (score > pa.finalscore.load())
				{
					if (verbosity > 1)
					{
						coutmtx.lock();
						cout << id << " NEW RECORD: " << result << " " << score << endl;
						coutmtx.unlock();
					}
					pa.assignmentfinishedmtx.lock(); //lock as final result isnt atomic
					pa.finalscore = score;
					pa.finalresult = result;
					pa.assignmentfinishedmtx.unlock();
				}

				pa.assignmentsfinished += assignmentsize;
				//pa.assignmentfinishedmtx.unlock();
			}
			else
			{
				pa.assignmentqueuemtx.unlock();
			}
		}
	} while (terminate.load() == false && numworkers > 1);
	quitmtx.lock();
	confirmquit++;
	quitmtx.unlock();
	if (verbosity == 2 || verbosity == 3)
	{
		coutmtx.lock();
		cout << "[Joker] ocr: worker " << id << " ended\n";
		coutmtx.unlock();
	}
}

tuple<string, long> ocr::solvepixelaverage(int mapbegin, int span, int id)
{
	long imglen = mymodel.pamodel.height * mymodel.pamodel.width;
	long looplen = span * imglen;
	long modelstart = mapbegin * imglen;

	long score = -500000;
	long tempscore = 0;
	int letter = 0;
	long counter1 = 0;
	long counter2 = 0;
	for (long iter = 0; iter < looplen; iter++) //*1 for now as one job is one comparison
	{
		tempscore += ((2*image.imgcontainer[counter1])-1) * mymodel.pamodel.model[(modelstart) + iter];
		counter1++;

		if (counter1 == imglen) //||counter==0
		{
			if (verbosity == 2 || verbosity == 3)
			{
				coutmtx.lock();
				cout << id << ": " << mymodel.pamodel.map[mapbegin + counter2] << " " << tempscore << "\n";
				coutmtx.unlock();
			}
			if (tempscore > score)
			{
				score = tempscore;
				letter = mapbegin + counter2;
			}
			tempscore = 0;
			counter1 = 0;
			counter2 += 1;

		}
	}

	return make_tuple(mymodel.pamodel.map[letter], score);
}

void ocr::endocr()
{
	terminate = true;
	if (verbosity == 2 || verbosity == 3)
	{
		cout << "[Joker] ocr: endocr: thread quit requested\n";
	}
	if (workercount != 1)
	{
		for (int tc = 0; tc < workercount; tc++)
		{
			threadpool[tc].join();
		}
	}
	while (confirmquit.load() < workercount)
	{
		continue;
	}
}
