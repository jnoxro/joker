//============================================================================
// Name        : trainer.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Train Joker models using various methodologies
//============================================================================

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include "joker.h"

using namespace std;

trainer::trainer()
{

}

void trainer::learnpixelavarage(string trainingdata, string modelname, int edgesup, int negativescore)
{

	cout << "[Joker] Pixel Average Training\n" << endl;

	ifstream mapfile(trainingdata + "/map.txt");
	if (!mapfile.is_open())
	{
		cerr << "[Joker] Error: Unable to open" << trainingdata << "/map.txt" << endl;
		exit(EXIT_FAILURE);
	}

	for(string line; getline( mapfile, line ); )
	{
	    mymodel.pamodel.map.push_back(line);
	}
	mapfile.close();

	cout << "[Joker] Library size:   " << mymodel.pamodel.map.size()-1 << endl;
	cout << "[Joker] File Extension: " << mymodel.pamodel.map[0] << endl;

	imgvect image;
	long imglen = 0;
	long currentlen = 0;
	long samplecounter = 0;


	for(long i = 1; i < (long)mymodel.pamodel.map.size(); i++)
	{
		while(1)
		{
			//cout << trainingdata << "/" << mymodel.pamodel.map[i] << "/" << to_string(samplecounter) << mymodel.pamodel.map[0] << endl;
			if (image.read(trainingdata + "/" + mymodel.pamodel.map[i] + "/" + to_string(samplecounter) + mymodel.pamodel.map[0], 125) == 1)
			{
				imglen = image.height() * image.width();
				currentlen = imglen*(i-1);

				for (long iter = 0; iter < imglen; iter++)
				{

					if ((long)mymodel.pamodel.model.size() - (currentlen+iter) < 1)
					{
						mymodel.pamodel.model.push_back(0);
					}
					mymodel.pamodel.model[currentlen + iter] = mymodel.pamodel.model[currentlen + iter] + image[iter];

				}
				samplecounter++;
			}
			else
			{

				cout << "[Joker] Trainer: loaded " << samplecounter << " samples for " << mymodel.pamodel.map[i] << endl;

				//map pixel values to between 0-1000
				float multiplier = 1000 / (float)samplecounter;
				for (long int iter = 0; iter < imglen; iter++) //for the newly read character
				{
					mymodel.pamodel.model[currentlen + iter] *= multiplier;
				}
				samplecounter = 0;
				break;
			}


		}


	}

	mymodel.setpixelaverage(image.height(), image.width(), negativescore);
	mymodel.save(modelname);
}
