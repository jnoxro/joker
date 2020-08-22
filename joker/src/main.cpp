//============================================================================
// Name        : main.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : super amazing fast OCR
//============================================================================

//TODO  main
// - create joker class (the ocr bit lol)
// - load model at start
// - if no args - open in wait mode and wait for file paths

//TODO Train
// - folders for each letter numbered from 0, contained images numbered from 0
// - start from folder 0, img 0, iterate img till !exists, iterate folder etc...
// - user provided map
// - create model from images
// - save model in loadable format
// - Decision trees with PCA? -> less operations to carry out maybe faster

//TODO Maybe
// - data set helper / creater

#include <cstring>
#include <string>
#include <iostream>
#include <Magick++.h>


#include "imggrab.h"

using namespace std;
using namespace Magick;

int main(int argc, const char* argv[]) {
	InitializeMagick(NULL);

	imggrab grabber;
	Image img;


	//defaults
	string filepath = "test.jpg";
	string modelpath = "model.jkr";
	string datapath = "data";
	string newmodel = "mymodel";
	int repeat = 0;
	int mode = 0;


	if (argc < 2){ 	//calling program is first arg

		cout << "[Joker] Welcome to Joker" << endl;

		cout << "\n[Joker] OCR launch flags:\n" << endl;
		cout << "[Joker] -o OCR mode       | -o" << endl;
		cout << "[Joker] -i image filepath | -i test.jpg" << endl;
		cout << "[Joker] -m model filepath | -m model.jkr" << endl;
		cout << "[Joker] -r repeat mode    | -r" << endl;

		cout << "\n[Joker] Training launch flags\n:" << endl;
		cout << "[Joker] -t train mode     | -t" << endl;
		cout << "[Joker] -d data folder    | -d data" << endl;
		cout << "[Joker] -n new model name | -n mymodel" << endl;
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
					cerr << "[Joker] can't use -o and -t at same time" << endl;
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
					if (argu1 == "-r")
					{
						repeat = 1;
					}
				}
			}
			if (argu == "-t")
			{
				if (mode == 1)
				{
					cerr << "[Joker] can't use -o and -t at same time" << endl;
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
				}
			}
		}
	}

	cout << filepath << endl;
	cout << modelpath << endl;
	cout << datapath << endl;
	cout << newmodel << endl;
	cout << repeat << endl;
	cout << mode << endl;

	if (grabber.grab(string(argv[1]))) //if able to load image
	{
		img = grabber.give();
		cout << "[Joker] hehe" << endl;

	}
	else
	{
		cout << "[Joker] no image!.. Exiting" << endl; // prints sah dude
		return 0;
	}


	return 0;
}
