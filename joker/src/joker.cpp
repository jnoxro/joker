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
	loadmodel();


	//threadpool:
//	if (threading == 1)
//	{
//		ctrlthreadpool(1);
//	}
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
			if (line == "map::")
			{
				readmode = 4;
				continue;
			}
			else
			{
				cerr << "[Joker] Error: Model format error map:: line:" << line << endl;
				exit(EXIT_FAILURE);
			}
		}
		if (readmode == 4)
		{
			if (line == "model::")
			{
				readmode = 5;
				continue;
			}
			else
			{
				map.push_back(line);
			}
		}
		if (readmode == 5)
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
					model.push_back(-1);
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

void joker::loadmodelold()
{

	auto loadstart = high_resolution_clock::now();


	ifstream input(modelname + ".jkr");
	if (!input.is_open())
	{
		cerr << "[Joker] Error: Unable to open model" << endl;
		exit(EXIT_FAILURE);
	}

	int readmode = 0;
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
			if (line == "map::")
			{
				readmode = 4;
				continue;
			}
			else
			{
				cerr << "[Joker] Error: Model format error map::" << endl;
				cout << line << endl;
				exit(EXIT_FAILURE);
			}
		}
		if (readmode == 4)
		{
			if (line == "model::")
			{
				readmode = 5;
				continue;
			}
			else
			{
				map.push_back(line);
			}
		}
		if (readmode == 5)
		{
			if (map.size() == 0)
			{
				cerr << "[Joker] Error: Model format error model::" << endl;
				cout << line << endl;
				exit(EXIT_FAILURE);
			}
			model.push_back(stol(line));
		}
	}

	if (verbose == 1)
	{
		auto loadstop = high_resolution_clock::now();
		auto loadduration = duration_cast<milliseconds>(loadstop - loadstart);
		cout <<  "model load time: "<< loadduration.count() << " millisecs" <<endl;
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
				//og threads:
				threadtest();

				//threadpool
				//dothreadwork();
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
			//og threads:
			threadtest();

			//threadpool
			//dothreadwork();
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

	int threadnum = thread::hardware_concurrency();

	if (verbose == 1 || verbose == 3)
	{
		cout << "[Joker] Possible concurrent threads: " << threadnum << endl;
	}

	vector<thread> workers;
	int size = map.size();

	for (unsigned int ms = 0; ms < map.size(); ms++)
	{
		threadoutputs.push_back(0); //expand output storage to correct size
	}

	for (int tc = 0; tc < threadnum; tc++)
	{
		int start = tc*(size/threadnum);
		int end = (tc*(size/threadnum) + (size/threadnum));
		if (tc == threadnum-1)
		{
			end = end + (size%threadnum);
		}
		workers.push_back(thread(&joker::ocrpixelavgthreaded, this, start, end, tc));
		//cout << tc << " " << start << " " << end << endl; //show task assignments
	}

	for (int tc = 0; tc < threadnum; tc++)
	{
		workers.at(tc).join();
	}

	long score = -100000;
	int pos = 0;

	for (unsigned int ms = 0; ms < map.size(); ms++)
	{
		//uncomment to see received high scores (1 per thread):
		//cout << map.at(ms) << " " << threadoutputs.at(ms) << endl;

		if (threadoutputs.at(ms) > score && threadoutputs.at(ms) != 0)
		{
			score = threadoutputs.at(ms);
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

void joker::ocrpixelavgthreaded(int start, int end, int id)
{

	mtx.lock();
	PixelPacket *pixels = image.getPixels(0, 0, image.columns(), image.rows());
	mtx.unlock();

	mtx1.lock();
	vector<int> submodel = { (model.begin() + (start*h*w)) , (model.begin() + (end*w*h)) };
	mtx1.unlock();

	long score = -100000;
	long tempscore = 0;
	int letter = 0;

	long counter1 = 0; //to image pixel count
	long counter2 = 0; //map pos counter
	for (long iter = 0; iter < w*h*(end-start); iter++)
	{
		tempscore = tempscore + (((((int)pixels[counter1].red)/255)*2)-1) * submodel.at(iter);
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

	mtx3.lock();
	threadoutputs[letter] = score;
	mtx3.unlock();

}

/*

void joker::ctrlthreadpool(int op)
{
	if (op == 1) //initiate threads
	{
		threadctl = 1;
		//max threads is 4 for now - set 8 vars (letter & score) so less locking required
		const int maxthread = 4;

		int threadnum = thread::hardware_concurrency();
		if (threadnum > maxthread)
		{
			threadnum = maxthread;
		}

		if (verbose == 1)
		{
			cout << "[Joker] Possible concurrent threads: " << threadnum << endl;
		}

		vector<thread> workers;

		for (int i = 0; i < threadnum; i++)
		{
			workers.push_back(thread(&joker::threadworker, this, i));
			workers.at(i).detach();
		}
	}

	if (op == 0)
	{
		threadctl = 0;
	}
}

void joker::threadworker(int id)
{
	int mappos = 0;
	int reuseimage = 0;
	int job = 0;
	int firstrun = 1;
	PixelPacket *pixels;

	while (threadctl == 1)
	{
		mtx1.lock();
		if (workqueue.size() == 0) //if workqueue is empty, then OCR is finished so expect a new image
		{
			reuseimage = 0;

		}
		if (workqueue.size() > 0)
		{
			mappos = workqueue.back();
			workqueue.pop_back();
			reuseimage = 1;
			job = 1;
		}
		mtx1.unlock();
		mtx2.lock();
		if ((job == 1 && reuseimage == 0) || (job == 1 && firstrun == 1))
		{
			pixels = image.getPixels(0, 0, image.columns(), image.rows());
		}
		mtx2.unlock();

		if (job == 1)
		{
			firstrun = 0;
			long counter1 = 0;
			long score = 0;

			for (long iter = 0; iter < w*h; iter++)
			{
				score = score + (((((int)pixels[counter1].red)/255)*2)-1) * model[(mappos*w*h)+iter];
				counter1++;

				if (counter1 == w*h)
				{
					mtx3.lock();
					threadreturn.at(mappos) = score;
					mtx3.unlock();
//					if (id == 0){threadreturn0.at(mappos) = score;}
//					if (id == 1){threadreturn1.at(mappos) = score;}
//					if (id == 2){threadreturn2.at(mappos) = score;}
//					if (id == 3){threadreturn3.at(mappos) = score;}
				}
			}
		}
	}
}

void joker::dothreadwork()
{
	auto ocrstart= high_resolution_clock::now();

	for (int i = 0; i < (int)map.size(); i++)
	{
		workqueue.push_back(i);
		threadreturn.push_back(0);
	}

	while (workqueue.size() != 0)
	{
		cout << workqueue.size() << endl;
		continue;
	}

	for (int i = 0; i < (int)map.size(); i++)
	{

		if (verbose == 2)
		{
			cout << map.at(i) << " | " << threadreturn.at(i) << endl;
		}
	}

	if (verbose == 1)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(ocrstop - ocrstart);
		cout <<  "[Joker] Raw threadpool OCR time: "<< duration.count() << " microsecs" <<endl;
	}

}

void joker::endocr()
{
	ctrlthreadpool(0);
}
*/
