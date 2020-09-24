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

	for (long unsigned int iter = 1; iter < pamodel.map.size(); iter++) //add map to file
	{
		output << pamodel.map[iter] << endl;
	}

	output << "::" << endl;
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
			if (lastval == -1) //if first loop then just write the first value.
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
	ifstream modelfile(modelname + ".jkr");
	if (!modelfile.is_open())
	{
		cerr << "[Joker] Error: Model: load: Unable to open" << modelname << ".jkr" << endl;
		exit(EXIT_FAILURE);
	}
	getline(modelfile, methodology);


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
	ifstream modelfile(modelname + ".jkr");
	if (!modelfile.is_open())
	{
		cerr << "[Joker] Error: Model: paload: Unable to open" << modelname << ".jkr" << endl;
		exit(EXIT_FAILURE);
	}

	int readmode = 0;
	long modelval = 0;
	int modellastval = 0;
	for(string line; getline(modelfile, line ); )
	{
		switch (readmode)
		{
		case 0:
			methodology = line;
			readmode = 1;
			break;

		case 1:
			pamodel.height = stoi(line);
			readmode = 2;
			break;

		case 2:
			pamodel.width = stoi(line);
			readmode = 3;
			break;

		case 3:
			pamodel.negativescore = stoi(line);
			readmode = 4;
			break;

		case 4:
			if (line != "::")
			{
				pamodel.map.push_back(line);
			}
			else
			{
				readmode = 5;
			}
			break;

		case 5:
			modelval = stol(line);
			if (modelval >= 0)
			{
				modellastval = (int)modelval;
				if (modelval == 0)
				{
					pamodel.model.push_back(pamodel.negativescore); //actual model values are 0-1000 for now so int
				}
				else
				{
					pamodel.model.push_back(modelval); //actual model values are 0-1000 for now so int
				}
			}
			else
			{
				for (long iter = 0; iter > modelval; iter--)
				{
					if (modellastval == 0)
					{
						pamodel.model.push_back(pamodel.negativescore); //actual model values are 0-1000 for now so int
					}
					else
					{
						pamodel.model.push_back(modelval); //actual model values are 0-1000 for now so int
					}
				}
			}
			break;

		default:
			cerr << "[Joker] Error: Model: paload: switch/case defaulted" << endl;
			modelfile.close();
			exit(EXIT_FAILURE);
		}
	}
	if (pamodel.model.size() == 0)
	{
		cerr << "[Joker] Warning: Model: paload: loaded model size is 0" << endl;
	}
	modelfile.close();
}
