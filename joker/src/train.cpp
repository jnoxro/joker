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
				//pixelColor is slow should use magick++ image cache
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

	cout << "\n[Joker] Model created, saving as: " << newmodel << ".jkr" << endl;

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
		output << model[h] << endl;
	}
	output.close();

	cout << "[Joker] Model saved." << endl;


/*
	int p = 0;
	int o = 0;
	for (int l = 0; l < 8000; l++)
	{
		cout << setw(3) << model.at(l);
		if (p == 40)
		{
			cout << "\n";
			p = 0;
		}
		if (o == 1600)
		{
			cout << "\n\n";
			o = 0;
		}
		o++;
		p++;
	}
*/


	/*
	for (int i = 0; i < map.size(); i++)
	{
		cout << map[i] << endl;
	}
	*/

	/*
	for (int v = 0; v < 40; v++)
	{
		cout << v << endl;
	}
	*/

}



