//============================================================================
// Name        : train.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Train Joker models using various methodologies
//============================================================================

#include <iostream>
#include <fstream>
#include <Magick++.h>

#include "train.h"
#include "imggrab.h"

using namespace std;
using namespace Magick;

train::train(std::string path, std::string name) //initialise
{
	datapath = path;
	newmodel = name;
}

void train::trainpixeladd()
{

}



