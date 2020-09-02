//============================================================================
// Name        : joker.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : OCR class, uses trained model to predict letter
//============================================================================


//TODO
// - Allow main script to call load model and init ocr separately


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

	if (verbose == 1)
	{
		auto loadstop = high_resolution_clock::now();
		auto loadduration = duration_cast<milliseconds>(loadstop - loadstart);
		cout <<  "model load time: "<< loadduration.count() << " millisecs" <<endl;
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
					//ocrpixelavgthreaded(0, 26, 99);
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
		auto duration = duration_cast<microseconds>(stop - start);
		cout <<  "Image load time: "<< duration.count() << " microsecs" <<endl;
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
	cout << score << endl;

	if (verbose == 1)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(ocrstop - ocrstart);
		cout <<  "Raw OCR time: "<< duration.count() << " millisecs" <<endl;
	}

}

void joker::threadtest()
{
	auto ocrstart = high_resolution_clock::now();

	int threadnum = thread::hardware_concurrency();

	if (verbose == 1)
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
		//cout << tc << " " << start << " " << end << endl;
	}

	for (int tc = 0; tc < threadnum; tc++)
	{
		workers.at(tc).join();
	}

	long score = -100000;
	int pos = 0;
	for (unsigned int ms = 0; ms < map.size(); ms++)
	{
		if (threadoutputs.at(ms) > score)
		{
			score = threadoutputs.at(ms);
			pos = ms;
		}
	}
	cout << map.at(pos) << " " << score << endl;

	if (verbose == 1)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(ocrstop - ocrstart);
		cout <<  "Raw threaded OCR time: "<< duration.count() << " millisecs" <<endl;
	}

}

void joker::ocrpixelavgthreaded(int start, int end, int id)
{

//	PixelPacket *pixels = test.getPixels(0,0,40,40);
//	cout << (int)pixels[w*h].red << endl;

//	vector<int> big_vector = {5,12,4,6,7,8,9,9,31,1,1,5,76,78,8};
//	vector<int> subvector = {big_vector.begin() + 3, big_vector.end() - 2};

//	pixcol = image.pixelColor(k,j);  //flipped j,k so model is height * width
//	tempscore = tempscore + (((2*pixcol.red())-1) * model.at((i*w*h)+(j*w)+k));

	mtx.lock();
	PixelPacket *pixels = image.getPixels(0, 0, image.columns(), image.rows());
	mtx.unlock();

	mtx.lock();
	vector<int> submodel = { (model.begin() + (start*h*w)) , (model.begin() + (end*w*h)) };
	mtx.unlock();

	long score = -100000;
	long tempscore = 0;
	int letter = 0;

	for (int i = 0; i < end-start; i++)
	{
		for (long iter = 0; iter < w*h; iter++)
		{
			tempscore = tempscore + (((((int)pixels[iter].red)/255)*2)-1) * submodel.at((i*w*h) + iter);
		}

		if (verbose == 1)
		{
			mtx.lock();
			cout << "Thread " << id << ":: 	" << map[start+i] << " | " << tempscore << endl;
			mtx.unlock();
		}

		if (tempscore > score)
		{
			score = tempscore;
			letter = start + i;
		}
	}
	mtx.lock();
	threadoutputs[letter] = score;
	mtx.unlock();


	/*
	ColorRGB pixcol;
	int score = -32000;
	int tempscore = 0;
	int letter = 0;
	for (int i = start; i < end; i++)
	{
		tempscore = 0;
		for (unsigned int j = 0; j < h; j++)
		{
			for (unsigned int k = 0; k < w; k++)
			{
				mtx.lock();
				pixcol = image.pixelColor(k,j);  //flipped j,k so model is height * width
				tempscore = tempscore + (((2*pixcol.red())-1) * model.at((i*w*h)+(j*w)+k));
				mtx.unlock();
			}
		}
		if (tempscore > score)
		{
			score = tempscore;
			letter  = i;
		}
		if (verbose == 1)
		{
			cout << "Thread " << id << ":: 	" << map[i] << " | " << tempscore << endl;
		}
	}
	mtx.lock();
	threadoutputs[letter] = score;
	mtx.unlock();
	//cout << letter << endl;
	//cout << score << endl;

	*/
}



