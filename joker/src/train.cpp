//============================================================================
// Name        : train.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Train Joker models using various methodologies
//============================================================================


//TODO
// .jkr file -> some arrangement of all matrices
// first lines of file specify matrix size, methodology type etc

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

	w = 0;
	h = 0;

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
	cout << "[Joker] File Extension: " << map[0] << endl;

	int totalimg = 0;
	ColorRGB pixcol;
	for (long unsigned int i = 1; i < map.size(); i++)
	{
		int counter = 0;
		while (1)
		{
			if (fetcher.grab(datapath + "/" + map[i] + "/" + to_string(counter) + map[0]) == 1)
			{
				image = fetcher.give();

				if (h == 0)
				{
					h = image.rows();
					w = image.columns();

					cout << "[Joker] Detected dimension: " << h << "*" << w << "\n" << endl;
				}
				else if (image.rows() != h || image.columns() != w)
				{
					cerr << "[Joker] Error: dimension mismatch: " << map[i] << "/" << counter  << map[0] << " not: " << h << "*" << w << endl;
					exit(EXIT_FAILURE);
				}

				image.threshold(125);

				//nasty code for now
				//pixelColor is slow should use magick++ image cache
				for (unsigned int j = 0; j < h; j++)
				{
					for (unsigned int k = 0; k < w; k++)
					{
						pixcol = image.pixelColor(j,k);

						if (pixcol.red() == 0)
						{
							if (counter == 0)
							{
								model.push_back(1);
							}
							else
							{
								model.at((j*(w))+k)++;
							}
						}
						else
						{
							if (counter == 0)
							{
								model.push_back(0);
							}
						}
					}
				}

				//ColorRGB pixcol = image.pixelColor(12,1);
				//cout << pixcol.red() << " " << pixcol.blue() << " " << pixcol.green() << endl;


				counter++;
			}
			else
			{
				totalimg = totalimg + counter;

				cout << "[Joker] Loaded " << counter << " samples for " << map[i] << endl;
				break;
			}

		}

	}
	cout << "model size: " << model.size() << endl;


	/*
	for (int l = 0; l < 1600; l++)
	{
		cout << model.at(l) << endl;
	}
	*/
	/*
	for (int i = 0; i < map.size(); i++)
	{
		cout << map[i] << endl;
	}
	*/

}



