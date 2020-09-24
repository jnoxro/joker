//============================================================================
// Name        : trainer.h
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Header file for model.cpp
//============================================================================

#ifndef MODEL_H_
#define MODEL_H_

#include <string>
#include <vector>

//model formats and their parameters:


class model
{
	private:

		//pamodelstruct pamodel;

		//model saves
		void pasave(std::string modelname); //pixelpverage save

		//model loads
		void paload(std::string modelname); //pixelaverage load



	public:
		model();

		//universal parameters
		std::string methodology;

		struct pamodelstruct //pixelaverage model
		{
			int width; //model character width
			int height; //model character height
			int negativescore = 0;
			std::vector<int> model; //model data
			std::vector<std::string> map;

		};
		pamodelstruct pamodel;

		void setpixelaverage(int w, int h, int n); //set the model type to pixel average and set struct params


		void save(std::string modelname); //saved model needs heigh, width, and negative scoring value
		void load(std::string modelname);

};



#endif /* MODEL_H_ */
