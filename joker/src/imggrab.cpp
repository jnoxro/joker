//============================================================================
// Name        : imggrab.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : Class for loading, storing and providing images to code
//============================================================================

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
    try
    {
    	image.read(file);  //read image
    	ret = 1;
    }
    catch(WarningFileOpen &warning)
	{
    	//cerr << "[Joker] Warning: " << warning.what() << endl;
    	ret = 0;
	}
    catch( Magick::ErrorFileOpen &error )
    {
    	//cerr << "[Joker] Error: " << error.what() << endl;
    	ret = 0;
    }


    return ret;
}


Image imggrab::give()
{
    return image;
}
