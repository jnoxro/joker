//============================================================================
// Name        : imgvect.cpp
// Author      : Jack Orton
// Version     :
// Copyright   : MIT License
// Description : custom vector form image type
//============================================================================


//TODO
// - Make threshold separate function
//

#include <string>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include "jpeglib.h"

#include "imgvect.h"

using namespace std;

imgvect::imgvect()
{
	w = 0;
	h = 0;
}

int imgvect::operator[] (int index)
{
	return imgcontainer[index];
}

int imgvect::read(string fname, int threshold)
{
	//find filetype, if jpeg then ingvect::readjpg
	istringstream iss(fname);
	string filetype;
	int success = 0;
	while (std::getline(iss, filetype, '.')) { continue; }

	if (filetype == "jpg")
	{
		success = readjpg(fname, threshold);
		//success = 1;
	}
	else
	{
		cerr << "[Joker] Error: Imgvect: unsupported image type: " << filetype << endl;
		success = 0;;
	}
	return success;

}

int imgvect::readjpg(string fname, int threshold)
{
	imgcontainer.clear(); //wipe previous image

	int retval = 0;
	struct jpeg_decompress_struct cinfo; //setup libjpeg for decompression
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE *infile;						//open image file
	const char * fnamec = fname.c_str();
	if ((infile = fopen(fnamec, "rb")) == NULL)
	{
		//fprintf(stderr, "[Joker] Error: can't open image");
		//cout << "[Joker] Error: Imgvect: unable to open " << fnamec << endl;
		retval = 0;
	}
	else
	{
		jpeg_stdio_src(&cinfo, infile); //begin decompression
		(void)jpeg_read_header(&cinfo, TRUE); //reads img properties
		jpeg_start_decompress(&cinfo);

		w = cinfo.output_width;
		h = cinfo.output_height;

		JSAMPARRAY buffer; //read line buffer
		buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, 1);

		int temp;
		while (cinfo.output_scanline < cinfo.output_height) //read each image pixel line
		{
			(void)jpeg_read_scanlines(&cinfo, buffer, 1);
			for (unsigned int i = 0; i < cinfo.output_width; i+=cinfo.output_components) //i+= to skip green and blue pixels.
			{
				temp = (int)(buffer[0][i]);
				if (threshold == 0)
				{
					imgcontainer.push_back(temp); //push value into
					retval = 1;
				}
				else if((temp >= threshold) && (threshold < 256))
				{
					imgcontainer.push_back(1); //pushing back 1-0 instead of 256-0
					retval = 1;
				}
				else if((temp < threshold) && (threshold > 0))
				{
					imgcontainer.push_back(0);
					retval = 1;
				}
				else
				{
					cerr << "[Joker] Error: invalid threshold val" << endl;
					exit(EXIT_FAILURE);
				}
			}

		}
		(void)jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
	}


	return retval;
}

int imgvect::width()
{
	return w;
}

int imgvect::height()
{
	return h;
}
