/*
 * imggrab.cpp
 *
 *  Created on: 9 Jul 2020
 *      Author: jack
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

#include "imggrab.h"

using namespace std;
using namespace cv;

imggrab::imggrab()
{
    ret = 0;
}

int imggrab::grab()
{
    image = imread("test/test.jpg");  //read image

    if (!image.data)
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    return ret;
}


Mat imggrab::give()
{
    return image;
}
