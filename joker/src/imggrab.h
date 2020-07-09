/*
 * imggrab.h
 *
 *  Created on: 9 Jul 2020
 *      Author: jack
 */

#ifndef IMGGRAB_H_
#define IMGGRAB_H_

#include <opencv2/core/core.hpp>
#include <string>

class imggrab
{

    private:
        int ret;    //load success/fail
        std::string file;	//filename
        cv::Mat image;  //read image

    public:
        imggrab();  //constructor

        int grab(std::string filename); //image grabber
        cv::Mat give(); //return

};


#endif /* IMGGRAB_H_ */
