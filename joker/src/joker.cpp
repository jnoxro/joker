//============================================================================
// Name        : joker.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : OCR class, uses trained model to predict letter
//============================================================================


//TODO
// - work queue of each letter score, thread pool works through


#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <Magick++.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "joker.h"
#include "imggrab.h"

using namespace std;
using namespace std::chrono;
using namespace Magick;

joker::joker(string modeln, int rep, int threadmode, std::string imgpath, int verb) //initialise
{
	modelname = modeln;
	filepath = imgpath;
	threading = threadmode;
	verbose = verb;
	repeat = rep;
	loadmodel();
	initocr();
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
			model.push_back(stoi(line));
		}
	}

	if (verbose == 1)
	{
		auto loadstop = high_resolution_clock::now();
		auto loadduration = duration_cast<milliseconds>(loadstop - loadstart);
		cout <<  "model load time: "<< loadduration.count() << " millisecs" <<endl;
	}

}

void joker::initocr()
{

	if (modeltype == "pixelaverage")
	{
		if (repeat == 0)
		{
			if (loadimage() == 1)
			{
				if (threading == 1)
				{
					threadtest();
				}
				else
				{
					ocrpixelavg();
				}
			}
		}
		else if (repeat == 1)
		{
			while (1)
			{
				cin >> filepath;
				if (filepath == "exit")
				{
					exit(EXIT_SUCCESS);
				}
				if (loadimage() == 1)
				{
					if (threading == 1)
					{
						threadtest();
					}
					else
					{
						ocrpixelavg();
					}
				}
			}
		}

	}
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

	if (verbose == 1)
	{
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		cout <<  "Image load time: "<< duration.count() << " millisecs" <<endl;
	}

	return 1;
}



void joker::ocrpixelavg()
{

	auto ocrstart = high_resolution_clock::now();

	ColorRGB pixcol;
	int score = -32000;
	int tempscore = 0;
	string letter;
	for (unsigned int i = 0; i < map.size(); i++)
	{
		tempscore = 0;
		for (unsigned int j = 0; j < h; j++)
		{
			for (unsigned int k = 0; k < w; k++)
			{
				pixcol = image.pixelColor(k,j);  //flipped j,k so model is height * width
				tempscore = tempscore + (((2*pixcol.red())-1) * model.at((i*w*h)+(j*w)+k));
			}
		}
		if (tempscore > score)
		{
			score = tempscore;
			letter  = map[i];
		}
		//cout << map[i] << " | " << tempscore << endl;
	}
	cout << letter << endl;
	//cout << score << endl;

	if (verbose == 1)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(ocrstop - ocrstart);
		cout <<  "Raw OCR time: "<< duration.count() << " millisecs" <<endl;
	}

}



//Threading is definitely broken different scores each run
void joker::threadtest()
{
	int threadnum = thread::hardware_concurrency();
	cout << "threading is broken dont use" << endl;
	cout << "possible concurrent threads: " << threadnum << endl;

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
		cout << tc << " " << start << " " << end << endl;
	}

	for (int tc = 0; tc < threadnum; tc++)
	{
		workers.at(tc).join();
	}


	/*
	thread worker1(&joker::ocrpixelavgthreaded, this, 0, 6, 1);
	thread worker2(&joker::ocrpixelavgthreaded, this, 7, 13, 2);
	thread worker3(&joker::ocrpixelavgthreaded, this, 14, 20, 3);
	thread worker4(&joker::ocrpixelavgthreaded, this, 21, 25, 4);

	worker1.join();
	worker2.join();
	worker3.join();
	worker4.join();
	*/

	/*
	for (unsigned int f = 0; f < threadoutputs.size(); f++)
	{
		cout << map.at(f) << " " << threadoutputs.at(f) << endl;

	}
	*/
}

void joker::ocrpixelavgthreaded(int start, int end, int id)
{
	auto ocrstart = high_resolution_clock::now();

	ColorRGB pixcol;
	unsigned int nh = image.rows();
	unsigned int nw = image.columns();
	int score = -32000;
	int tempscore = 0;
	int letter;
	for (int i = start; i < end; i++)
	{
		tempscore = 0;
		for (unsigned int j = 0; j < nh; j++)
		{
			for (unsigned int k = 0; k < nw; k++)
			{
				pixcol = image.pixelColor(k,j);  //flipped j,k so model is height * width
				tempscore = tempscore + (((2*pixcol.red())-1) * model.at((i*nw*nh)+(j*nw)+k));
			}
		}
		if (tempscore > score)
		{
			score = tempscore;
			letter  = i;
		}
		cout << "Thread " << id << ":: 	" << map[i] << " | " << tempscore << endl;
	}
	threadoutputs[letter] = score;
	//cout << letter << endl;
	//cout << score << endl;

	if (verbose == 1)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(ocrstop - ocrstart);
		cout <<  "Raw OCR time: "<< duration.count() << " millisecs" <<endl;
	}

}



