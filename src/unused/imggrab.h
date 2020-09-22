//============================================================================
// Name        : imggrab.h
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Header file for imaggrab.cpp
//============================================================================

#ifndef IMGGRAB_H_
#define IMGGRAB_H_

#include <Magick++.h>
#include <string>

class imggrab
{

    private:
        int ret;    //load success/fail
        std::string file;	//filename
        Magick::Image image;  //read image

    public:
        imggrab();  //constructor

        int grab(std::string filename); //image grabber
        Magick::Image give(); //return

};


#endif /* IMGGRAB_H_ */
