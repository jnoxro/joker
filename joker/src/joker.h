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

		void loadmodel();

    public:
        joker(std::string modeln);  //constructor



};


#endif /* JOKER_H_ */
