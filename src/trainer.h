//============================================================================
// Name        : trainer.h
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Header file for trainer.cpp
//============================================================================

#ifndef TRAINER_H_
#define TRAINER_H_

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>

#include "joker.h"
#include "model.h"

class trainer
{
	private:

		//std::string modelname; //name to save model as
		//std::string trainingdata; //file path for trainingdata
		//std::string methodology; //ocr&learning methodology

		model mymodel;



	public:
		trainer();

		void learnpixelavarage(std::string trainingdata, std::string modelname, int edgesup, int negativescore);

};



#endif /* TRAINER_H_ */
