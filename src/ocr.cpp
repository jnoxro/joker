//============================================================================
// Name        : ocr.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : OCR class, uses trained model to predict letter
//============================================================================

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <string>
#include <atomic>
#include <chrono>

#include "ocr.h"
#include "imgvect.h"
#include "model.h"

using namespace std;
using namespace std::chrono;

ocr::ocr(string modelname, int threads, int verbose)
{
	verbosity = verbose;

	auto stop = high_resolution_clock::now();
	mymodel.load(modelname);
	if (verbose == 1 || verbose == 3)
	{
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout <<  "[Joker] Ocr: model load time: "<< duration.count() << " microsecs" << endl;
	}
}
