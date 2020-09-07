# Joker

Joker is an OCR engine created with the focus of high speed on low performance systems

## Installation

Joker is uploaded to git with the necessary source files to be compiled as a standalone, command line program, or can be integrated into existing C++ projects

### Pre-compiled Binaries

Binaries for your system may be available on the [Releases](https://github.com/jnoxro/joker/releases) page. 



### Compiling from source

Joker can be compiled from source using cmake, download the latest tagged release code from the main [repo page](https://github.com/jnoxro/joker)

GraphicsMagick is required for compilation, along with its dependencies.

During development, GraphicsMagick 1.3.35 was used so it is recommended this version or later is installed on the compiling system. Other than GraphicsMagick, only standard C++ libraries are used, as such required libraries can be seen with the Linux command line using:

`GraphicsMagick++-config --libs`

Once these libraries are installed, navigate to the Joker directory and type the following commands for the simplest install:

`mkdir Build`

`cd Build`

`cmake ..`

`make`

This will produce a 'joker' executable within the Build folder.

By default -O3 compiler optimizations are applied. 


## Usage & Performance 

To launch the joker welcome screen and view available commands use

`./joker`

This will show the available flags which can be used to control training and OCR. Visit the [Wiki](https://github.com/jnoxro/joker/wiki) for further launch information

Current tests on an Intel® Pentium(R) CPU N3710 @ 1.60GHz × 4 processor result in a single threaded OCR time of ~275 microseconds (0.275 milliseconds, 0.000275 seconds) for a 40*40pixel library size with 26 characters using the PixelAveraging OCR method.
