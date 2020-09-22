//============================================================================
// Name        : imgvect.h
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Header file for imgvect.cpp
//============================================================================

#ifndef IMGVECT_H_
#define IMGVECT_H_

#include <vector>
#include <string>

class imgvect
{
	private:
		std::vector<int> imgcontainer;
		int readjpg(std::string fname, int threshold);
		int w;
		int h;

	public:
		imgvect(); //constructor
		int operator[](int); //allow index access to imgcontainer
		int read(std::string fname, int threshold);

		int width();
		int height();
};



#endif /* IMGVECT_H_ */
