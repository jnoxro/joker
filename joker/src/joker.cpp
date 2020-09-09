//============================================================================
// Name        : joker.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : OCR class, uses trained model to predict letter
//============================================================================


//TODO
//


#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <Magick++.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

#include "joker.h"
#include "imggrab.h"

using namespace std;
using namespace std::chrono;
using namespace Magick;

joker::joker(string modeln, int threadmode,  int verb) //initialise
{
	modelname = modeln;

	threading = threadmode;
	verbose = verb;

	threadcount = 0;
	newwork = 0; //signifies new work available
	queue = 0; //queue would just hold numbers of map positions, instead threads can just reduce int by 1 to take a job.
	terminator = 0; //set to 1 to end worker loops
	loadmodel();

	initthreadpool();
}

void joker::loadmodel()
{

	auto loadstart = high_resolution_clock::now();


	ifstream input(modelname + ".jkr");
	if (!input.is_open())
	{
		cerr << "[Joker] Error: Unable to open model" << endl;
		exit(EXIT_FAILURE);
	}

	int readmode = 0;
	long inint;
	long modelcounter = 0;
	int negscore = -1;
	for(string line; getline( input, line ); )
	{
		if (readmode == 0)
		{
			modeltype = line;
			readmode = 1;
			continue;
		}
		if (readmode == 1)
		{
			h = stoi(line);
			readmode = 2;
			continue;
		}
		if (readmode == 2)
		{
			w = stoi(line);
			readmode = 3;
			continue;
		}
		if (readmode == 3)
		{
			negscore = stoi(line);
			readmode = 4;
			continue;
		}
		if (readmode == 4)
		{
			if (line == "map::")
			{
				readmode = 5;
				continue;
			}
			else
			{
				cerr << "[Joker] Error: Model format error map:: line:" << line << endl;
				exit(EXIT_FAILURE);
			}
		}
		if (readmode == 5)
		{
			if (line == "model::")
			{
				readmode = 6;
				continue;
			}
			else
			{
				map.push_back(line);
			}
		}
		if (readmode == 6)
		{
			if (map.size() == 0)
			{
				cerr << "[Joker] Error: Model format error model:: line:" << line << endl;
				exit(EXIT_FAILURE);
			}

			inint = stol(line);
			if (inint >= 0) //fill in model white space
			{
				for (int mc = modelcounter; mc < inint; mc++)
				{
					model.push_back(negscore);
					modelcounter++;
				}
			}
			else
			{
				model.push_back(inint);
				modelcounter++;
			}

		}
	}
	if (model.size() != (h*w*map.size())) //missing info
	{
		for (long unsigned int mc = modelcounter; mc < (h*w*map.size()); mc++)
		{
			model.push_back(-1);
		}
	}
	//cout << model.size() << endl;

	if (verbose == 1 || verbose == 3)
	{
		auto loadstop = high_resolution_clock::now();
		auto loadduration = duration_cast<milliseconds>(loadstop - loadstart);
		cout <<  "[Joker] Model load time: "<< loadduration.count() << " millisecs" <<endl;
	}

}

string joker::initocr(string imagepath)
{
	filepath = imagepath;

	if (modeltype == "pixelaverage")
	{
		if (loadimage() == 1)
		{
			if (threading == 1)
			{

				//threadtest();

				job(); //add job to threadpool
			}
			else
			{
				ocrpixelavg();
			}
		}
	}

	return result;
}

string joker::initocr(Image newimage)
{
	image = newimage;

	if (modeltype == "pixelaverage")
	{
		if (threading == 1)
		{

//			threadtest();

			job();

		}
		else
		{
			ocrpixelavg();
		}
	}
	return result;
}

int joker::loadimage()
{

	auto start = high_resolution_clock::now();


	imggrab fetcher;
	if (fetcher.grab(filepath) == 0)
	{
		cerr << "[Joker] Error: image load fail" << endl;
		exit(EXIT_FAILURE);
		return 0;
	}
	else
	{
		image = fetcher.give();

		image.threshold(125);
		//image.negate();
		if (image.rows() != h || image.columns() != w)
		{
			cerr << "[Joker] Error: Image does not match model dimensions" << endl;
			exit(EXIT_FAILURE);
		}
	}

	if (verbose == 1 || verbose == 3)
	{
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout <<  "[Joker] Image load time: "<< duration.count() << " microsecs" <<endl;
	}

	return 1;
}

void joker::ocrpixelavg()
{
	auto ocrstart = high_resolution_clock::now();
	PixelPacket *pixels = image.getPixels(0, 0, image.columns(), image.rows());

	long score = -100000;
	long tempscore = 0;
	int letter = 0;

	long counter1 = 0; //to image pixel count
	long counter2 = 0; //map pos counter
	for (long iter = 0; iter < w*h*((int)map.size()); iter++)
	{
		tempscore = tempscore + (((((int)pixels[counter1].red)/255)*2)-1) * model.at(iter);
		counter1++;

		if (counter1 == w*h) //||counter==0
		{
			if (tempscore > score)
			{
				score = tempscore;
				letter =counter2;
			}
			if ((verbose == 2 || verbose == 3) && counter1 > 0)
			{
				cout << "[Joker] "<< map[counter2] << " | " << tempscore << endl;
			}
			if (counter1 != 0)
			{
				counter2++;
			}
			tempscore = 0;
			counter1 = 0;

		}

	}

	//cout << map.at(letter) << endl;
	result = map.at(letter);

	if (verbose == 1 || verbose == 3)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(ocrstop - ocrstart);
		cout << "[Joker] Raw OCR time: "<< duration.count() << " microsecs" <<endl;
	}

}


//Experimental::

void joker::threadtest()
{
	auto ocrstart = high_resolution_clock::now();

	PixelPacket *pixels = image.getPixels(0, 0, image.columns(), image.rows());

	int threadnum = thread::hardware_concurrency();
	if (verbose == 1 || verbose == 3)
	{
		cout << "[Joker] Possible concurrent threads: " << threadnum << endl;
	}

	if (verbose == 1 || verbose == 3)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(ocrstop - ocrstart);
		cout <<  "[Joker] 1: "<< duration.count() << " microsecs" <<endl;
	}

	vector<thread> workers;
	int size = map.size();
	for (unsigned int ms = 0; ms < map.size(); ms++)
	{
		threadoutputs.push_back(0); //expand output storage to correct size
	}

	if (verbose == 1 || verbose == 3)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(ocrstop - ocrstart);
		cout <<  "[Joker] 2: "<< duration.count() << " microsecs" <<endl;
	}


	for (int tc = 0; tc < threadnum; tc++)
	{
		int start = tc*(size/threadnum);
		int end = (tc*(size/threadnum) + (size/threadnum));
		if (tc == threadnum-1)
		{
			end = end + (size%threadnum);
		}
		workers.push_back(thread(&joker::ocrpixelavgthreaded, this, start, end, tc, pixels));
		//cout << tc << " " << start << " " << end << endl; //show task assignments
	}

	if (verbose == 1 || verbose == 3)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(ocrstop - ocrstart);
		cout <<  "[Joker] 3: "<< duration.count() << " microsecs" <<endl;
	}

	for (int tc = 0; tc < threadnum; tc++)
	{
		workers[tc].join();
	}

	if (verbose == 1 || verbose == 3)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(ocrstop - ocrstart);
		cout <<  "[Joker] 4: "<< duration.count() << " microsecs" <<endl;
	}

	long score = -100000;
	int pos = 0;

	for (unsigned int ms = 0; ms < map.size(); ms++)
	{
		//uncomment to see received high scores (1 per thread):
		//cout << map.at(ms) << " " << threadoutputs.at(ms) << endl;

		if (threadoutputs[ms] > score && threadoutputs[ms] != 0)
		{
			score = threadoutputs[ms];
			pos = ms;
		}
	}
	//cout << map.at(pos) << endl;
	result = map.at(pos);

	if (verbose == 1 || verbose == 3)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(ocrstop - ocrstart);
		cout <<  "[Joker] Raw threaded OCR time: "<< duration.count() << " microsecs" <<endl;
	}

}

void joker::ocrpixelavgthreaded(int start, int end, int id, PixelPacket *pixels)
{

	auto startt = high_resolution_clock::now();

	/*
	mtx.lock();
	PixelPacket *pixels = image.getPixels(0, 0, image.columns(), image.rows()); //30-50 microsecs
	mtx.unlock();
	*/

	/*
	mtx1.lock();
	vector<int> submodel = { (model.begin() + (start*h*w)) , (model.begin() + (end*w*h)) }; //this function alone taking 200+ microsecs
	mtx1.unlock();
	*/


	long score = -100000;
	long tempscore = 0;
	int letter = 0;

	long counter1 = 0; //to image pixel count
	long counter2 = 0; //map pos counter
	for (long iter = 0; iter < w*h*(end-start); iter++)
	{
		//tempscore = tempscore + (((((int)pixels[counter1].red)/255)*2)-1) * submodel.at(iter);
		tempscore = tempscore + (((((int)pixels[counter1].red)/255)*2)-1) * model[iter + (w*h*start)];
		counter1++;

		if (counter1 == w*h) //||counter==0
		{
			if (tempscore > score)
			{
				score = tempscore;
				letter = start + counter2;
			}
			if ((verbose == 2 || verbose == 3) && counter1 > 0)
			{
				mtx2.lock();
				cout << "[Joker] Thread " << id << ": 	" << map[start+counter2] << " | " << tempscore << endl;
				mtx2.unlock();
			}
			if (counter1 != 0)
			{
				counter2++;
			}
			tempscore = 0;
			counter1 = 0;

		}


	}

	//mtx3.lock();
	threadoutputs[letter] = score;
	//mtx3.unlock();

	if (verbose == 1 || verbose == 3)
	{
		auto stopt = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stopt - startt);
		cout <<  "[Joker] Thread " << id << " time: "<< duration.count() << " microsecs" <<endl;
	}
}


//very experimental::

void joker::initthreadpool()
{
	threadcount = thread::hardware_concurrency();

	if (verbose == 1 || verbose == 3)
	{
		cout << "[Joker] Possible concurrent threads: " << threadcount << endl;
	}

	for (int tc = 0; tc < threadcount; tc++)
	{
		workers.push_back(thread(&joker::worker, this, tc)); //tc acts as thread id
	}
}

void joker::worker(int id)
{
	if (verbose == 1 || verbose == 3)
	{
		poolmtx.lock();
		cout << "[Joker] Thread " << id << " started" << endl;
		poolmtx.unlock();
	}

	int mapj = 0;
	int newj = 0;
	int newimg = 1;
	PixelPacket *pixels;

	long score = -100000;
	long tempscore = 0;
	long counter1 = 0;
	int letter;

	while (terminator == 0)
	{
		if (newwork.load() == 1) //maybe mutex around if declare
		{
			poolmtx0.lock();
			if (newimg == 1)
			{
				pixels = image.getPixels(0, 0, image.columns(), image.rows()); //could be moved to initocr so only done once
				newimg = 0;
			}
			poolmtx0.unlock();

			poolmtx1.lock();
			if (queue > 0)
			{
				queue--;
				mapj = queue; //after queue-- as would have been mapj = queue-1 anyways (queue = mapsize, not indexes)
				newj = 1; //signify queue was bigger than 0
				if (queue == 0)
				{
					--newwork;
				}
			}
			poolmtx1.unlock();

			if (newj == 1)
			{
				for (long iter = 0; iter < w*h*(1); iter++) //*1 for now as one job is one comparison
				{
					//tempscore = tempscore + (((((int)pixels[counter1].red)/255)*2)-1) * submodel.at(iter);
					tempscore = tempscore + (((((int)pixels[counter1].red)/255)*2)-1) * model[iter + (w*h*mapj)];
					counter1++;

					if (counter1 == w*h) //||counter==0
					{
						if (tempscore > score)
						{
							score = tempscore;
							letter = mapj;
						}
						if ((verbose == 2 || verbose == 3) && counter1 > 0)
						{
							poolmtx3.lock();
							cout << "[Joker] Thread " << id << ": 	" << map[mapj] << " | " << tempscore << endl;
							poolmtx3.unlock();
						}
						tempscore = 0;
						counter1 = 0;

					}
				}

				poolmtx4.lock();
				//if score>globalscore, resut=letter mutexed
				if (score > globalscore)
				{
					globalscore = score;
					result = map[letter];
				}
				poolmtx4.unlock();

				score = -100000;
				++fin;
			}
		}
		if (newwork.load() == 0)
		{
			newimg = 1;
		}
	}

	if (verbose == 1 || verbose == 3)
	{
		poolmtx2.lock();
		cout << "[Joker] Thread " << id << " Ended" << endl;
		poolmtx2.unlock();
	}
}

void joker::job()
{
	auto startt = high_resolution_clock::now();

	queue = map.size();
	++newwork;


	while (fin.load() < map.size())
	{
		continue;
	}

	if (verbose == 1 || verbose == 3)
	{
		auto stopt = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stopt - startt);
		cout <<  "[Joker] pool job time: "<< duration.count() << " microsecs" <<endl;
	}

}

void joker::endocr()
{
	terminator = 1;
	for (int tc = 0; tc < threadcount; tc++)
	{
		workers[tc].join();
	}
}
