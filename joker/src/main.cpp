//============================================================================
// Name        : joker.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : its free baby
// Description : super amazing fast OCR
//============================================================================

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "imggrab.h"

using namespace std;
using namespace cv;

int main() {

	imggrab grabber;
	Mat img;

	if (grabber.grab())
	{
		img = grabber.give();
		/*
		imshow( "Display window", img );                   //show grabbed img
		waitKey(0);
		*/



	}
	else
	{
		cout << "no image!" << endl; // prints sah dude
		return 0;
	}


	return 0;
}
