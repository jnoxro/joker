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
#include <mutex>

class joker
{

    private:
		std::vector<long> model;
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
		void loadmodelold();
		int loadimage();
		void ocrpixelavg();



		// Experimental thread testing::

		//each thread outputs the score for a letter in the letters respective map position
		//EG if threadoutputs.at(4) = 5000 then the letter at map.at(4) has that score
		// https://stackoverflow.com/questions/45720829/can-two-threads-write-to-different-element-of-the-same-array
		int threading;

		std::mutex mtx; //mutex for locking threads
		std::mutex mtx1;
		std::mutex mtx2;
		std::mutex mtx3;
		std::vector<int> threadoutputs;

		void threadtest();
		void ocrpixelavgthreaded(int start, int end, int id);


		// thread testing 2 - thread pool
		/*

		std::vector<int> workqueue;
		std::vector<int> threadreturn;
//		std::vector<int> threadreturn0;
//		std::vector<int> threadreturn1;
//		std::vector<int> threadreturn2;
//		std::vector<int> threadreturn3;
		int threadctl;

		void ctrlthreadpool(int op); //initiate concurrent workers - call in constructor
		void threadworker(int id); //workers wait for work added to queue
		void dothreadwork(); //add work to queue and get result
		*/

    public:

        joker(std::string modeln, int rep, int threadmode, int verb);  //constructor

        void initocr(std::string imagepath);
        void initocr(Magick::Image newimage); //overload allows image to be directly passed

        void endocr();



};


#endif /* JOKER_H_ */
