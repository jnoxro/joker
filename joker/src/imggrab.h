/*
 * imggrab.h
 *
 *  Created on: 9 Jul 2020
 *      Author: jack
 */

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
