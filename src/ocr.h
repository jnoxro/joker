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
#include <mutex>
#include <tuple>
#include <thread>

#include "imgvect.h"
#include "model.h"
#include "timer.h"

class ocr
{

    private:
		timer timer1; //timer to time functions
		model mymodel; //model handler
		imgvect image; //image handler

		int verbosity = 0; //control what is displayed, 1 = times, 2 = ocr scores, 3 = both


		void initthreadpool(int threads); //create threads which wait for jobs
		std::atomic<int> threadready{0};

		void job(); //add job

		void worker(int id, int numworkers); //worker carries out job
		std::vector<std::thread> threadpool;
		std::mutex coutmtx;
		std::atomic<int> confirmquit{0};
		std::mutex quitmtx;

		bool extrathreads = false; //signify if external threads will be working on job
		int workercount = 1; //amount of workers
		std::atomic<bool> terminate; //used to signal threads to close


		//methodologies:
		struct pajob // all things needed for pixelaveraging
		{
			std::atomic<int> assignmentqueue{0}; //map indexes which still need comparing, reduces from (mapsize-1) -> 0, reduced by thread assignment size (mapsize/(worker count))
			std::mutex assignmentqueuemtx; //mutex to check queue

			std::atomic<int> assignmentsfinished{0}; //amount of assignments completed, when this = map.size then job is finished
			std::string finalresult = ""; //best result
			std::atomic<long> finalscore{-500000}; //preset score very low to prevent it being higher than any upcoming results
			std::mutex assignmentfinishedmtx; //mutex check results
		};
		pajob pa;
		std::tuple<std::string, long> solvepixelaverage(int mapbegin, int span, int id); //method for pixelaverage solving, used by one or many threads


    public:
		ocr(std::string modelname, int threads, int verbose); //constructor

		std::tuple<std::string, long> solve(std::vector<int> target); //user can pass custom vector or imgvect.imgcontainer
		std::tuple<std::string, long> solve(std::string filepath); //user can pass filepath to open image with imgvect.cpp



		void endocr(); //terminate the threadpool

};


#endif /* OCR_H_ */
