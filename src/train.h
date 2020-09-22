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
//#include <Magick++.h>
#include <vector>

#include "imgvect.h"
//#include "imggrab.h"

class train
{

    private:
		std::string newmodel;
		std::string datapath;
		std::string traintype;

		unsigned int h;
		unsigned int w;
		int negscore;

		//imggrab fetcher;
		imgvect image;

		//Magick::Image image;

		std::vector<std::string> map;
		std::vector<int> model;

		void savemodel();
		void savemodelold(std::string mode);

		//methodologies:
		void trainpixelavg(unsigned int edgesup); //og pixel addition

    public:
		train(std::string path, std::string name); //constructor

		void learn(std::string method, int egdesup, int neg);
};


#endif /* TRAIN_H_ */
