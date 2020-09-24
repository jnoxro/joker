//============================================================================
// Name        : ocr.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : OCR class, uses trained model to predict letter
//============================================================================


//TODO
//


#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
//#include <Magick++.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

#include "joker.h"
//#include "ocr.h"
//#include "imgvect.h"
//#include "imggrab.h"

using namespace std;
using namespace std::chrono;
//using namespace Magick;

ocr::ocr(string modeln, int threadmode,  int verb) //initialise
{

	threading = threadmode;
	verbose = verb;

	threadcount = 0;
	newwork = 0; //signifies new work available
	queue = 0; //queue would just hold numbers of map positions, instead threads can just reduce int by 1 to take a job.
	terminator = 0; //set to 1 to end worker loops

	auto start = high_resolution_clock::now();
	mymodel.load(modeln);
	if (verbose == 1 || verbose == 3)
	{
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout <<  "[Joker] Ocr: model load time: "<< duration.count() << " microsecs" << endl;
	}
	w = mymodel.pamodel.width;
	h = mymodel.pamodel.height;

	/*
	cout << "model info:" << endl;
	cout << "method: " << mymodel.methodology << endl;
	cout << "w*h: " << w << "*" << h << endl;
	cout << "map & model sizes: " << mymodel.pamodel.map.size() << " " << mymodel.pamodel.model.size() << endl;
	cout << "expected model size: " << w*h*26 << endl;
	 */

	if (threading == 1)
	{
		initthreadpool();
	}

}

/*
void ocr::loadmodel()
{

	auto loadstart = high_resolution_clock::now();


	ifstream input(modelname + ".jkr");
	if (!input.is_open())
	{
		cerr << "[Joker] Error: Unable to open model" << endl;
		exit(EXIT_FAILURE);
	}

	int readmode = 0;
	long inint;
	long modelcounter = 0;
	int negscore = -1;
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
			h = stoi(line);
			readmode = 2;
			continue;
		}
		if (readmode == 2)
		{
			w = stoi(line);
			readmode = 3;
			continue;
		}
		if (readmode == 3)
		{
			negscore = stoi(line);
			readmode = 4;
			continue;
		}
		if (readmode == 4)
		{
			if (line == "map::")
			{
				readmode = 5;
				continue;
			}
			else
			{
				cerr << "[Joker] Error: Model format error map:: line:" << line << endl;
				exit(EXIT_FAILURE);
			}
		}
		if (readmode == 5)
		{
			if (line == "model::")
			{
				readmode = 6;
				continue;
			}
			else
			{
				map.push_back(line);
			}
		}
		if (readmode == 6)
		{
			if (map.size() == 0)
			{
				cerr << "[Joker] Error: Model format error model:: line:" << line << endl;
				exit(EXIT_FAILURE);
			}

			inint = stol(line);
			if (inint >= 0) //fill in model white space
			{
				for (int mc = modelcounter; mc < inint; mc++)
				{
					model.push_back(negscore);
					modelcounter++;
				}
			}
			else
			{
				model.push_back(inint);
				modelcounter++;
			}

		}
	}
	if (model.size() != (h*w*map.size())) //missing info
	{
		for (long unsigned int mc = modelcounter; mc < (h*w*map.size()); mc++)
		{
			model.push_back(-1);
		}
	}
	//cout << model.size() << endl;

	if (verbose == 1 || verbose == 3)
	{
		auto loadstop = high_resolution_clock::now();
		auto loadduration = duration_cast<milliseconds>(loadstop - loadstart);
		cout <<  "[Joker] Model load time: "<< loadduration.count() << " millisecs" <<endl;
	}

}
*/

string ocr::initocr(string imagepath)
{
	if (mymodel.methodology == "pixelaverage")
	{
		if (loadimage(imagepath) == 1)
		{
			if (threading == 1)
			{
				//threadtest();
				job(); //add job to threadpool and wait till done
			}
			else
			{
				ocrpixelavg();
			}
		}

		else
		{
			cout << "an odd occurance" << endl;
			result = "";
		}
	}

	return result;
}


int ocr::loadimage(string imagepath)
{

	auto start = high_resolution_clock::now();

	int retval = 0;

	switch (image.read(imagepath, 125))
	{
	case 1:
		if(image.height() != h || image.width() != w)
		{
			cerr << "[Joker] Error: ocr: loadimage: Image does not match model dimensions" << endl;
			retval = 0;
		}
		else
		{
			retval = 1;
		}
		break;

	case 0:
		cerr << "[Joker] Error: ocr: loadimage: image load fail" << endl;
		retval = 0;
		break;

	default:
		cerr << "[Joker] Error: ocr: loadimage: unknown image load output - aborting" << endl;
		exit(EXIT_FAILURE);
	}


	if (verbose == 1 || verbose == 3)
	{
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout <<  "[Joker] OCR: Image load time: "<< duration.count() << " microsecs" << endl;
	}

	return retval;
}

void ocr::ocrpixelavg()
{
	auto ocrstart = high_resolution_clock::now();
	//PixelPacket *pixels = image.getPixels(0, 0, image.columns(), image.rows());

	long score = -100000;
	long tempscore = 0;
	int letter = 0;

	long counter1 = 0; //to image pixel count
	long counter2 = 0; //map pos counter

	long iterend = w*h*((int)mymodel.pamodel.map.size()); //moved out of loop for less calcs
	long imagelength = w*h;

	for (long iter = 0; iter < iterend; iter++)
	{
		//tempscore = tempscore + (((((int)pixels[counter1].red)/255)*2)-1) * model[iter];
		//tempscore = tempscore + ((2*image[counter1])-1) * mymodel.pamodel.model[iter];
		//tempscore += (2*(image[counter1])-1) * mymodel.pamodel.model[iter];
		tempscore += (2*(image.imgcontainer[counter1])-1) * mymodel.pamodel.model[iter];
		counter1++;


		if (counter1 == imagelength) //||counter==0
		{
			if (tempscore > score)
			{
				score = tempscore;
				letter = counter2;
			}
			if ((verbose == 2 || verbose == 3) && counter1 > 0)
			{
				cout << "[Joker] "<< mymodel.pamodel.map[counter2] << " | " << tempscore << endl;
			}
			if (counter1 != 0)
			{
				counter2++;
			}
			tempscore = 0;
			counter1 = 0;

		}

	}

	//cout << map.at(letter) << endl;
	result = mymodel.pamodel.map[letter];

	if (verbose == 1 || verbose == 3)
	{
		auto ocrstop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(ocrstop - ocrstart);
		cout << "[Joker] Raw OCR time: "<< duration.count() << " microsecs" <<endl;
	}

}


//very experimental::

void ocr::initthreadpool()
{
	threadcount = thread::hardware_concurrency();

	if (verbose == 1 || verbose == 3)
	{
		cout << "[Joker] Possible concurrent threads: " << threadcount << endl;
	}

	for (int tc = 0; tc < threadcount; tc++)
	{
		workers.push_back(thread(&ocr::worker, this, tc, threadcount)); //tc acts as thread id
	}
}

void ocr::worker(int id, int of)
{
	if (verbose == 1 || verbose == 3)
	{
		poolmtx.lock();
		cout << "[Joker] Thread " << id << " started" << endl;
		poolmtx.unlock();
	}

	int mapj = 0; //work map position
	int newj = 0; //work left
	int worksize = 0; //size of work (how much of map being covered)
	//int newimg = 1; //new image required (new job)
	//PixelPacket *pixels;

	long score = -100000;
	long tempscore = 0;
	long counter1 = 0;
	long counter2 = 0;
	int letter = 0;

	while (terminator == 0)
	{
		if (newwork.load() == 1) //maybe mutex around if declare
		{

			poolmtx1.lock();
			if (queue > 0)
			{
				if (queue > 1)
				{
					queue -= 2;
					mapj = queue; //after queue-- as would have been mapj = queue-1 anyways (queue = mapsize, not indexes)
					newj = 2; //work left
					worksize = 2;
				}
				else
				{
					queue -= 1;
					mapj = queue; //after queue-- as would have been mapj = queue-1 anyways (queue = mapsize, not indexes)
					newj = 1; //work left
					worksize = 1;
				}
				if (queue == 0)
				{
					newwork = 0;
				}
			}
			poolmtx1.unlock();

			long modelstart = w*h*(mapj); //moved out loop to reduce calcs
			long iterend = w*h*(worksize); //moved out of loop to reduce calcs
			long imagelength = w*h; //moved out of loop to reduce calcs
			if (newj > 0)
			{
				for (long iter = 0; iter < iterend; iter++) //*1 for now as one job is one comparison
				{
					//tempscore = tempscore + (((((int)poolpixels[counter1].red)/255)*2)-1) * model[(modelstart) + iter];
					//tempscore = tempscore + ((2*image[counter1])-1) * mymodel.pamodel.model[(modelstart) + iter];
					tempscore += ((2*image.imgcontainer[counter1])-1) * mymodel.pamodel.model[(modelstart) + iter];
					counter1++;

					if (counter1 == imagelength) //||counter==0
					{
						if (tempscore > score)
						{
							score = tempscore;
							letter = mapj + counter2;
						}
						if ((verbose == 2 || verbose == 3) && counter1 > 0)
						{
							poolmtx3.lock();
							cout << "[Joker] Thread " << id << ": 	" << mymodel.pamodel.map[mapj + counter2] << " | " << tempscore << endl;
							poolmtx3.unlock();
						}
						tempscore = 0;
						counter1 = 0;
						counter2 += 1;

					}
				}

				poolmtx4.lock();
				//if score>globalscore, resut=letter mutexed
				if (score > globalscore)
				{
					globalscore = score;
					result = mymodel.pamodel.map[letter];
				}
				poolmtx4.unlock();

				//reset ready for next use
				counter2 = 0;
				score = -100000;
				fin += worksize;
			}
		}
//		if (newwork.load() == 0)
//		{
//			newimg = 1;
//		}
	}

	if (verbose == 1 || verbose == 3)
	{
		poolmtx2.lock();
		cout << "[Joker] Thread " << id << " Ended" << endl;
		poolmtx2.unlock();
	}
}

void ocr::job()
{
	//adds work to queue, needs updating to add image to stack as well as splitting the map per image
	auto startt = high_resolution_clock::now();

	//poolpixels = image.getPixels(0, 0, image.columns(), image.rows());

	queue = mymodel.pamodel.map.size();
	newwork = 1;
	while (fin.load() < queue)
	{
		continue;
	}

	globalscore = -100000; //reset to avoid no result when all negs=ative scores
	if (verbose == 1 || verbose == 3)
	{
		auto stopt = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stopt - startt);
		cout <<  "[Joker] pool job time: "<< duration.count() << " microsecs" <<endl;
	}

}

void ocr::endocr()
{
	terminator = 1;
	for (int tc = 0; tc < threadcount; tc++)
	{
		workers[tc].join();
	}
}
