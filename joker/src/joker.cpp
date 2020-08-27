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

joker::joker(string modeln, int mode, std::string imgpath) //initialise
{
	modelname = modeln;
	filepath = imgpath;
	loadmodel();
}

void joker::loadmodel()
{

	//cout << "loading model" << endl;

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

	//cout << "loaded model" << endl;
	//cout << "model size: " << model.size() << endl;

	if (modeltype == "pixelaverage")
	{
		if (loadimage() == 1)
		{
			auto start = high_resolution_clock::now();


			//ocrpixelavg();

			//cout << "\nThread test:\n" << endl;
			threadtest();

			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<milliseconds>(stop - start);

			cout <<  "Raw OCR time: "<< duration.count() << " millisecs" <<endl;
		}

	}


}



int joker::loadimage()
{
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

	return 1;
}



void joker::ocrpixelavg()
{

	ColorRGB pixcol;
	int score = 0;
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
		cout << map[i] << " | " << tempscore << endl;
	}
	cout << letter << endl;
	//cout << score << endl;

}


void joker::threadtest()
{
	int threadnum = thread::hardware_concurrency();
	cout << "possible concurrent threads: " << threadnum << endl;

	for (unsigned int ms = 0; ms < map.size(); ms++)
	{
		threadoutputs.push_back(0); //expand output storage to correct size
	}

	thread worker1(&joker::ocrpixelavgthreaded, this, 0, 6, 1);
	thread worker2(&joker::ocrpixelavgthreaded, this, 7, 13, 2);
	thread worker3(&joker::ocrpixelavgthreaded, this, 14, 20, 3);
	thread worker4(&joker::ocrpixelavgthreaded, this, 21, 25, 4);

	worker1.join();
	worker2.join();
	worker3.join();
	worker4.join();

	for (unsigned int f = 0; f < threadoutputs.size(); f++)
	{
		cout << map.at(f) << " " << threadoutputs.at(f) << endl;

	}
}

void joker::ocrpixelavgthreaded(int start, int end, int id)
{

	ColorRGB pixcol;
	int score = 0;
	int tempscore = 0;
	int letter;
	for (int i = start; i <= end; i++)
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
			letter  = i;
		}
		cout << "Thread " << id << ":: 	" << map[i] << " | " << tempscore << endl;
	}
	threadoutputs[letter] = score;
	//cout << letter << endl;
	//cout << score << endl;

}



