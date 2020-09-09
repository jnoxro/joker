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
#include <atomic>

class joker
{

    private:
		std::string result;
		long globalscore;

		std::vector<long> model;
		std::vector<std::string> map;

		std::string modelname;
		std::string modeltype;

		std::string filepath;

		Magick::Image image;

		int verbose;

		unsigned int h;
		unsigned int w;

		void loadmodel();
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
		void ocrpixelavgthreaded(int start, int end, int id, Magick::PixelPacket *pixels);




		//*********//
		//threadpooling should use resources better

		std::vector<std::thread> workers;

		std::mutex poolmtx;
		std::mutex poolmtx0;
		std::mutex poolmtx1;
		std::mutex poolmtx2;
		std::mutex poolmtx3;
		std::mutex poolmtx4;

		int threadcount;
		//int newwork; //signifies new work available
		int queue; //queue would just hold numbers of map positions, instead threads can just reduce int by 1 to take a job.
		int terminator; //set to 1 to end worker loops
		std::atomic<int> newwork{0};
		std::atomic<int> fin{0};

		Magick::PixelPacket *poolpixels;

		void initthreadpool();
		void worker(int id);
		void job();

    public:

        joker(std::string modeln, int threadmode, int verb);  //constructor

        std::string initocr(std::string imagepath);
        std::string initocr(Magick::Image newimage); //overload allows image to be directly passed

        void endocr();



};


#endif /* JOKER_H_ */
