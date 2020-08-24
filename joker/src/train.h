//============================================================================
// Name        : train.h
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Header file for train.cpp
//============================================================================

#ifndef TRAIN_H_
#define TRAIN_H_

#include <string>
#include <Magick++.h>
#include <vector>

#include "imggrab.h"

class train
{

    private:
		std::string newmodel;
		std::string datapath;

		unsigned int h;
		unsigned int w;

		imggrab fetcher;

		Magick::Image image;
		//Magick::ColorMono pixval;

		std::vector<std::string> map;


    public:
		train(std::string path, std::string name); //constructor

		//methodologies:
		void trainpixelavg(); //og pixel addition
};


#endif /* TRAIN_H_ */
