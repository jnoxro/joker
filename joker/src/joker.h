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
#include <thread>

class joker
{

    private:
		std::vector<int> model;
		std::vector<std::string> map;

		std::string modelname;
		std::string modeltype;

		std::string filepath;

		Magick::Image image;

		int verbose;
		int repeat;

		unsigned int h;
		unsigned int w;

		void loadmodel();
		int loadimage();
		void initocr();
		void ocrpixelavg();



		// thread testing

		//each thread outputs the score for a letter in the letters respective map position
		//EG if threadoutputs.at(4) = 5000 then the letter at map.at(4) has that score
		// https://stackoverflow.com/questions/45720829/can-two-threads-write-to-different-element-of-the-same-array
		int threading;
		std::vector<int> threadoutputs;

		void threadtest();
		void ocrpixelavgthreaded(int start, int end, int id);

    public:
        joker(std::string modeln, int rep, int threadmode, std::string imgpath, int verb);  //constructor



};


#endif /* JOKER_H_ */
