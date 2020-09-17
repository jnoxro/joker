# Joker

Joker is an OCR engine created with the focus of high speed on low performance systems

## Installation

Joker is uploaded to git with the necessary source files to be compiled as a standalone, command line program, or can be integrated into existing C++ projects

### Pre-compiled Binaries

Binaries for your system may be available on the [Releases](https://github.com/jnoxro/joker/releases) page. 



### Compiling from source

Joker can be compiled from source using cmake, download the latest tagged release code from the main [repo page](https://github.com/jnoxro/joker).

Cmake will downlaod and install a version of GraphicsMagick into 'External/' within the project files (this is not a system wide install and should not intefere with any other installs you have) which it will then use when building Joker.

From with the joker project file, run the following commands in terminal:

`mkdir Build`

`cd Build`

`cmake ..`

`make`

This will produce a 'joker' executable within the Build folder.



## Usage & Performance 

To launch the joker welcome screen and view available commands use

`./joker`

This will show the available flags which can be used to control training and OCR. Visit the [Wiki](https://github.com/jnoxro/joker/wiki) for further launch information

Current tests on an Intel® Pentium(R) CPU N3710 @ 1.60GHz × 4 processor result in a single threaded OCR time of ~275 microseconds (0.275 milliseconds, 0.000275 seconds) for a 40*40pixel library size with 26 characters using the PixelAveraging OCR method.
