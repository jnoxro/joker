# Joker

Joker is a character recognition engine created with the focus of high speed

## Installation

Joker is uploaded to git with the necessary source files to be compiled as a standalone, command line program, or can be integrated into existing C++ projects

### Pre-compiled Binaries

Binaries for your system may be available on the [Releases](https://github.com/jnoxro/joker/releases) page. 



### Compiling from source

Joker can be compiled from source using cmake, download the latest tagged release code from the main [repo page](https://github.com/jnoxro/joker).

Make sure you have the following shared libraries inatalled:
 - libjpeg-turbo-dev (often preinstalled on linux, current ubuntu: libjpeg-turbo8-dev)
 - pthread (often preinstalled on linux)

From within the joker project file, run the following commands in terminal:

`mkdir Build`

`cd Build`

`cmake ..`

`make`

This will produce a 'jk' executable within the Build folder wich can be ran with `./jk` from within /Build.

`make install`  - this may require sudo

This will install the joker libraries and executable to /usr/local. Joker can now be invoked with `jk` in terminal.



## Usage & Performance 

To launch the joker welcome screen and view available commands use

`jk`

This will show the available flags which can be used to control training and OCR. Visit the [Wiki](https://github.com/jnoxro/joker/wiki) for further launch information

Current tests on an Intel® Pentium(R) CPU N3710 @ 1.60GHz × 4 processor result in a single threaded OCR time of <200 microseconds (0.2 milliseconds, 0.0002 seconds) for a 40*40pixel library size with 26 characters using the PixelAveraging OCR method.
