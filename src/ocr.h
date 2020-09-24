//============================================================================
// Name        : ocr.h
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Header file for ocr.cpp
//============================================================================

#ifndef OCR_H_
#define OCR_H_

#include <vector>
#include <string>
#include <atomic>

#include "imgvect.h"
#include "model.h"

class ocr
{

    private:

		model mymodel; //model handler
		imgvect image; //image handler

		int verbosity = 0; //control cout's
		bool extrathreads = false; //signify if external threads will be working on job

		void solvepixelaverage(std::vector<long> target, int mapbegin, int mapend); //method for pixelaverage solving, used by one or many threads

		std::atomic<bool> terminate; //used to signal threads to close
		void initthreadpool(int threads);

		void addjob(); //add job to queue
		void worker(int id, int numworkers); //worker carries out job

		//note for threads
		//threads user definable in ocr call (below)
		//0 - auto threadcount
		//1 - run in main script (dont make any threads)
		//2+ - make that many threads
    public:
		ocr(std::string modelname, int threads, int verbose); //constructor

		void solve(std::vector<long> target); //user can pass custom vector or imgvect.imgcontainer
		void solve(std::string filepath); //user can pass filepath to open image with imgvect.cpp

		std::string finalresult = ""; //user accessible results
		long finalscore = -500000;

		int endocr(); //terminate the threadpool

};


#endif /* OCR_H_ */
