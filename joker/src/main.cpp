//============================================================================
// Name        : main.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : super amazing fast OCR
//============================================================================

//TODO/notes  main
// - create joker class (the ocr bit lol)
// - load model at start

//TODO/notes Train
// - folders for each letter numbered from 0, contained images numbered from 0
// - start from folder 0, img 0, iterate img till !exists, iterate folder etc...
// - user provided map
// - create model from images
// - save model in loadable format
// - Decision trees with PCA? -> less operations to carry out maybe faster

//TODO/notes Maybe
// - data set helper / creater

#include <string>
#include <iostream>
#include <Magick++.h>
#include <chrono>

#include "train.h"
#include "joker.h"

using namespace std;
using namespace std::chrono;
using namespace Magick;

int main(int argc, const char* argv[]) {
	InitializeMagick(NULL);

	//defaults//
	string filepath = "test.jpg";
	string modelpath = "mymodel";
	string datapath = "data";
	string newmodel = "mymodel";
	string method = "pixelaverage";
	int threadmode = 0;
	int verbose = 0;
	int mode = 0;
	int e = 0;
	int negscore = -1;

	void welcome();

	//argy parsy//
	if (argc < 2)
	{
		welcome();
		return 0;
	}
	else
	{
		for (int count = 1; count < argc; count++)
		{
			string argu = argv[count];
			if (argu == "-o")
			{
				if (mode == 2)
				{
					cerr << "[Joker] Error: can't use -o and -t at same time" << endl;
					return 0;
				}
				mode = 1;
				for (int count1 = 1; count1 < argc; count1++)
				{
					string argu1 = argv[count1];
					if (argu1 == "-i")
					{
						filepath = argv[count1+1];
					}
					if (argu1 == "-m")
					{
						modelpath = argv[count1+1];
					}
					if (argu1 == "-thread")
					{
						threadmode = 1;
					}
					if (argu1 == "-v")
					{
						verbose = stoi(argv[count1+1]);
					}
				}
			}
			if (argu == "-t")
			{
				if (mode == 1)
				{
					cerr << "[Joker] Error: can't use -o and -t at same time" << endl;
					return 0;
				}
				mode = 2;
				for (int count2 = 0; count2 < argc; count2++)
				{
					string argu2 = argv[count2];
					if (argu2 == "-d")
					{
						datapath = argv[count2+1];
					}
					if (argu2 == "-n")
					{
						newmodel = argv[count2+1];
					}
					if (argu2 == "-s")
					{
						method = argv[count2+1];
					}
					if (argu2 == "-e")
					{
						e = stoi(argv[count2+1]);
					}
					if (argu2 == "-p") //p for penalty
					{
						negscore = stoi(argv[count2+1]);
					}
				}
			}
		}
	}

	if (mode == 2)
	{
		train trainer(datapath, newmodel);
		trainer.learn(method, e, negscore);
	}

	if (mode == 1)
	{


		auto start = high_resolution_clock::now();

		joker ocr(modelpath, threadmode, verbose);
		string letter = ocr.initocr(filepath);
		cout << letter << endl;

		if (verbose == 1)
		{
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<milliseconds>(stop - start);
			cout << "[Joker] Total time: " << duration.count() << " millisecs" <<endl;
		}

	}



/*
	Image test;
	imggrab grabby;
	grabby.grab("trainingData5/A/0.jpg");
	test = grabby.give();
	test.threshold(125);
	//Pixels view(test);

	auto cachestart = high_resolution_clock::now();
	PixelPacket *pixels = test.getPixels(0,0,40,40);
	cout << (int)pixels[34].red << endl;
	auto cachestop = high_resolution_clock::now();
	auto cacheduration = duration_cast<microseconds>(cachestop - cachestart);
	cout <<  "cache time: "<< cacheduration.count() << " microsecs" <<endl;

	auto ogstart = high_resolution_clock::now();
	ColorRGB pixcol;
	pixcol = test.pixelColor(34,0);
	cout << pixcol.red() << endl;
	auto ogstop = high_resolution_clock::now();
	auto ogduration = duration_cast<microseconds>(ogstop - ogstart);
	cout <<  "og time: "<< ogduration.count() << " microsecs" <<endl;

*/

	return 0;
}



void welcome()
{
	cout << "[Joker] Welcome to Joker" << endl;
	cout << R"(
   ___         _                
  |_  |       | |               
    | |  ___  | | __  ___  _ __ 
    | | / _ \ | |/ / / _ \| '__|
/\__/ /| (_) ||   < |  __/| |   
\____/  \___/ |_|\_\ \___||_| )" << endl;


	cout << "\n[Joker] OCR launch flags:\n" << endl;
	cout << "[Joker] -o OCR     		| -o" << endl;
	cout << "[Joker] -thread threading	| -thread" << endl;
	cout << "[Joker] -v verbose	level   | -v 1" << endl;
	cout << "[Joker] -i image filepath	| -i test.jpg" << endl;
	cout << "[Joker] -m model name		| -m mymodel" << endl;


	cout << "\n[Joker] Training launch flags:\n" << endl;
	cout << "[Joker] -t train          | -t" << endl;
	cout << "[Joker] -s methodology    | -s pixelaverage" << endl;
	cout << "[Joker] -e edge suppress  | -e 5" << endl;
	cout << "[Joker] -p negative score | -p -1" << endl;
	cout << "[Joker] -d data folder    | -d data" << endl;
	cout << "[Joker] -n new model name | -n mymodel" << endl;

	cout << "\n\n[Joker] Jack Orton	  https://github.com/jnoxro/joker" << endl;
}
