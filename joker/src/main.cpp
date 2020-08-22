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

#include <iostream>
#include <Magick++.h>

#include "imggrab.h"

using namespace std;
using namespace Magick;

int main(int argc, const char* argv[]) {
	InitializeMagick(NULL);

	imggrab grabber;
	Image img;

	if (argc < 2){ 	//calling program is first arg
		cout << "[Joker] need more args:" << endl;
		cout << "[Joker] launch with ./joker filepath" << endl;
		return 0;
	}

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
