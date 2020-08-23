//============================================================================
// Name        : train.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Train Joker models using various methodologies
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <Magick++.h>
#include <vector>
#include <string>

#include "train.h"
#include "imggrab.h"

using namespace std;
using namespace Magick;


train::train(std::string path, std::string name) //initialise
{
	datapath = path;
	newmodel = name;

	cout << "[Joker] Training mode" << endl;
	cout << "\n[Joker] Data path: " << datapath << endl;
	cout << "[Joker] New model name: " << newmodel << endl;
}


void train::trainpixelavg()
{
	cout << "\n[Joker] Pixel Average training" << endl;

	ifstream input(datapath + "/map.txt");
	if (!input.is_open())
	{
		cerr << "[Joker] Error: Unable to open map" << endl;
		exit(EXIT_FAILURE);
	}

	for(string line; getline( input, line ); )
	{
	    map.push_back(line);
	}
	input.close();

	cout << "[Joker] Library size:   " << map.size()-1 << endl;
	cout << "[Joker] File Extension: " << map[0] << "\n" << endl;

	for (long unsigned int i = 1; i < map.size(); i++)
	{
		int counter = 0;
		while (1)
		{
			if (fetcher.grab(datapath + "/" + map[i] + "/" + to_string(counter) + map[0]) == 1)
			{
				image = fetcher.give();
				counter++;
			}
			else
			{
				counter--;
				cout << "[Joker] Loaded " << counter << " samples for " << map[i] << endl;
				break;
			}

			//if total colours > 2
			//	//adaptive threshold

		}
	}



	/*
	for (int i = 0; i < map.size(); i++)
	{
		cout << map[i] << endl;
	}
	*/

}



