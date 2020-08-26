//============================================================================
// Name        : joker.h
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Header file for joker.cpp
//============================================================================

#ifndef JOKER_H_
#define JOKER_H_

#include <Magick++.h>
#include <string>
#include <vector>

class joker
{

    private:
		std::vector<int> model;
		std::vector<std::string> map;

		std::string modelname;
		std::string modeltype;

		std::string filepath;
		Magick::Image image;

		unsigned int h;
		unsigned int w;


		void loadmodel();
		void ocrpixelavg();

    public:
        joker(std::string modeln, int mode, std::string imgpath);  //constructor



};


#endif /* JOKER_H_ */
