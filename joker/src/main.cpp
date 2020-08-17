//============================================================================
// Name        : joker main.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : its free baby
// Description : super amazing fast OCR
//============================================================================

//TODO
// - create joker class (the ocr bit lol)
// - load model at start
// - if no args - open in wait mode and wait for file paths

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
		cout << "need more args:" << endl;
		cout << "./joker filepath" << endl;
		return 0;
	}

	if (grabber.grab(string(argv[1]))) //if able to load image
	{
		img = grabber.give();
		cout << "hehe" << endl;


	}
	else
	{
		cout << "no image!" << endl; // prints sah dude
		return 0;
	}


	return 0;
}
