//============================================================================
// Name        : trainer.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Model object to be used by train.cpp
//============================================================================

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include "joker.h"

using namespace std;

model::model()
{
	methodology = "pixelaverage"; //default to pixelaverage
}

void model::setpixelaverage(int w, int h, int n)
{
	methodology = "pixelaverage";
	pamodel.height = h;
	pamodel.width = w;
	pamodel.negativescore = n;

}


void model::save(string modelname)
{

	if (methodology == "pixelaverage")
	{
		if (pamodel.model.size() > 0 && pamodel.height != NULL && pamodel.width != NULL)
		{
			pasave(modelname);
		}
		else
		{
			cerr << "Joker: Error: Model: missing information for save" << endl;
			exit(EXIT_FAILURE);
		}
	}

	else
	{
		cerr << "Joker: Error: Model: unknown save method for methodology: " << methodology << endl;
		exit(EXIT_FAILURE);
	}

}

void model::pasave(string modelname)
{
	ofstream output(modelname + ".jkr"); //open model file and save data
	output << methodology << endl;
	output << pamodel.height << endl;
	output << pamodel.width << endl;
	output << pamodel.negativescore << endl;

	output << "map::" << endl;
	for (long unsigned int iter = 1; iter < pamodel.map.size(); iter++) //add map to file
	{
		output << pamodel.map[iter] << endl;
	}

	output << "model::" << endl;
	long val;
	long lastval = -1; //set to below min val to signify first run of loop
	long repeatcount = 0; //how many MORE times val is repeated after first
	long iter = 0;
	long saved = 0;
	while (1)
	{
		val = pamodel.model[iter];
		//if (val == 0) val = pamodel.negativescore;
		if (val == lastval) //to reduce file size, hence load time, save to file values, and how often they occur
		{
			repeatcount++;
		}
		else
		{
			if (lastval < 1) //if first loop then just write the first value.
			{
				output << val << endl;
				lastval = val; saved++;
			}
			else //if not first loop and new char is different
			{
				output << lastval << endl; //output last char
				if (repeatcount > 0) output << -repeatcount << endl;
				saved = saved + repeatcount + 1;
				repeatcount = 0;
				lastval = val;
			}
		}

		if (saved == pamodel.model.size()-1) //if last loop
		{
			if (repeatcount > 0)
			{
				output << lastval << endl;
				output << -repeatcount << endl;
			}
			else
			{
				output << val << endl;
			}
			break;
		}

		iter++;
	}
	output.close();
}

void model::load(string modelname)
{
	if (methodology == "pixelaverage")
	{
		paload(modelname);
	}
	else
	{
		cerr << "Joker: Error: Model: unknown load method for methodology: " << methodology << endl;
		exit(EXIT_FAILURE);
	}
}

void model::paload(string modelname)
{

}
