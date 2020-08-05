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
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "imggrab.h"

using namespace std;
using namespace cv;

int main(int argc, const char* argv[]) {

	imggrab grabber;
	Mat img;

	if (argc < 2){ 	//calling program is first arg
		cout << "need more args:" << endl;
		cout << "./joker filepath" << endl;
		return 0;
	}

	if (grabber.grab(string(argv[1]))) //if able to load image
	{
		img = grabber.give();
		cout << "hehe" << endl;

		imshow( "Display window", img );                   //show grabbed img
		waitKey(0);



	}
	else
	{
		cout << "no image!" << endl; // prints sah dude
		return 0;
	}


	return 0;
}
