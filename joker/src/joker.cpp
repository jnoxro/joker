//============================================================================
// Name        : joker.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : OCR class, uses trained model to predict letter
//============================================================================

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <Magick++.h>
#include <vector>
#include <string>

#include "joker.h"
#include "imggrab.h"

using namespace std;

joker::joker(string modeln) //initialise
{
	modelname = modeln;
	loadmodel();
}

void joker::loadmodel()
{
	ifstream input(modelname);
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
			if (line == "map::")
			{
				readmode = 2;
				continue;
			}
			else
			{
				cerr << "[Joker] Error: Model format error" << endl;
				exit(EXIT_FAILURE);
			}
		}
		if (readmode == 2)
		{
			if (line == "model::")
			{
				readmode = 3;
				continue;
			}
			else
			{
				map.push_back(line);
			}
		}
		if (readmode == 3)
		{
			if (map.size() == 0)
			{
				cerr << "[Joker] Error: Model format error" << endl;
				exit(EXIT_FAILURE);
			}
			model.push_back(stoi(line));
		}
	}
}


