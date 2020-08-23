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

class train
{

    private:
		std::string newmodel;
		std::string datapath;

		Magick::Image image;


    public:
		train(std::string path, std::string name); //constructor



		//methodologies:
		void trainpixeladd(); //og pixel addition


};


#endif /* TRAIN_H_ */
