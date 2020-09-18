//============================================================================
// Name        : train.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Train Joker models using various methodologies
//============================================================================


//TODO
// edge suppression

#include <iostream>
#include <iomanip>
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
	negscore = -1;

	cout << "[Joker] Training mode" << endl;
	cout << "\n[Joker] Data path: " << datapath << endl;
	cout << "[Joker] New model name: " << newmodel << endl;
}

void train::learn(std::string method, int edgesup, int neg)
{
	traintype = method;
	negscore = neg;

	if (traintype == "pixelaverage")
	{
		trainpixelavg(edgesup);
	}
	else
	{
		cerr << "[Joker] Error: Unrecognised methodology: " << traintype << endl;
		exit(EXIT_FAILURE);
	}
}


void train::trainpixelavg(unsigned int edgesup)
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

	ColorRGB pixcol;
	int lettercount = 0;
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
				//pixelColor on image itself is slow should use image cache
				for (unsigned int j = 0; j < h; j++)
				{
					for (unsigned int k = 0; k < w; k++)
					{
						pixcol = image.pixelColor(k,j);  //flipped j,k so model is height * width

						if (pixcol.red() == 0)
						{
							if (counter == 0)
							{
								model.push_back(1);
							}
							else
							{
								model.at((lettercount*h*w)+(j*w)+k)++;

								if (j < edgesup || k < edgesup || j > (h-edgesup) || k > (w-edgesup))
								{
									model.at((lettercount*h*w)+(j*w)+k) = model.at((lettercount*h*w)+(j*w)+k) * 0.2;
								}
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
				cout << "[Joker] Loaded " << counter << " samples for " << map[i] << endl;
				lettercount++;
				break;
			}

		}

	}

	cout << "\n[Joker] Model created" << endl;

	savemodel();


}

void train::savemodel()
{
	if (traintype == "pixelaverage")
	{
		ofstream output(newmodel + ".jkr");
		output << traintype << endl;
		output << h << endl;
		output << w << endl;
		output << negscore << endl;
		output << "map::" << endl; //map marker
		for (long unsigned int g = 1; g < map.size(); g++)
		{
			output << map[g] << endl;
		}
		output << "model::" << endl; //model marker

		int lastval = 0; //0 means empty pixel, 1 means info
		for (long unsigned int h = 0; h < model.size(); h++)
		{
			if (model[h] == 0)
			{
				lastval = 0;
			}
			else
			{
				if (lastval == 0)
				{
					output << h << endl; //h will be a positive number so can be distinguished in model from pixel info
					lastval = 1;
				}
				output << -(model[h]) << endl; //-(m) for negative scoring for lack of overlap
			}
		}
		output.close();

		cout << "[Joker] Model saved" << endl;
	}

}

void train::savemodelold(string mode)
{
	if (mode == "pixelaverage")
	{
		ofstream output(newmodel + ".jkr");
		output << "pixelaverage" << endl;
		output << h << endl;
		output << w << endl;
		output << "map::" << endl; //map marker
		for (long unsigned int g = 1; g < map.size(); g++)
		{
			output << map[g] << endl;
		}
		output << "model::" << endl; //model marker
		for (long unsigned int h = 0; h < model.size(); h++)
		{
			output << -(model[h] + 1) << endl; //-(m+1) for negative scoring in white space and non overlap
		}
		output.close();

		cout << "[Joker] Model saved" << endl;
	}
}

