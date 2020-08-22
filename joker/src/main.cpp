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
	char filepath[] = "test.jpg";
	char modelpath[] = "model.jkr";
	char datapath[] = "data";
	char newmodel[] = "mymodel";
	int repeat = 0;
	int mode = 0;


	if (argc < 2){ 	//calling program is first arg

		cout << strcmp(filepath, "tesjt.jpg") << endl;

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
			if (strcmp(argv[count],"-o") == 0)
			{
				if (mode == 2)
				{
					cout << "[Joker] can't use -o and -t at same time" << endl;
					break;
					return 0;
				}
				mode = 1;
				for (int count1 = 1; count1 < argc; count1++)
				{
					if (strcmp(argv[count1],"-i") == 0)
					{
						strcpy(filepath,argv[count1+1]);
					}
					if (strcmp(argv[count1],"-m") == 0)
					{
						strcpy(modelpath,argv[count1+1]);
					}
					if (strcmp(argv[count1],"-r") == 0)
					{
						repeat = 1;
					}
				}
				break;
			}
			if (strcmp(argv[count],"-t") == 0)
			{
				if (mode == 1)
				{
					cout << "[Joker] can't use -o and -t at same time" << endl;
					break;
					return 0;
				}
				mode = 2;
				for (int count2 = 0; count2 < argc; count2++)
				{
					if (strcmp(argv[count2],"-d") == 0)
					{
						strcpy(datapath,argv[count2+1]);
					}
					if (strcmp(argv[count2],"-n") == 0)
					{
						strcpy(newmodel,argv[count2+1]);
					}
				}
				break;
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
