/*
 * imggrab.cpp
 *
 *  Created on: 9 Jul 2020
 *      Author: jack
 */

#include <iostream>
#include <Magick++.h>

#include "imggrab.h"

using namespace std;
using namespace Magick;

imggrab::imggrab() //initialise
{
	ret = 0;
}

int imggrab::grab(string filename) //set to "" for release
{

    file = filename;
    image.read(file);  //read image

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


Image imggrab::give()
{
    return image;
}
