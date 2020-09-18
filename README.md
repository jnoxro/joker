# Joker

Joker is a character recognition engine created with the focus of high speed on low performance systems

## Installation

Joker is uploaded to git with the necessary source files to be compiled as a standalone, command line program, or can be integrated into existing C++ projects

### Pre-compiled Binaries

Binaries for your system may be available on the [Releases](https://github.com/jnoxro/joker/releases) page. 



### Compiling from source

Joker can be compiled from source using cmake, download the latest tagged release code from the main [repo page](https://github.com/jnoxro/joker).

Cmake will download and install a version of GraphicsMagick into 'External/' within the project files (this is not a system wide install and should not intefere with any other installs you have) which it will then use when building Joker.

From within the joker project file, run the following commands in terminal:

`mkdir Build`

`cd Build`

`cmake ..`

optional cmake -D flags:
- GM_VERSION=x.x.x specify a specific version on GraphicsMagick to be downloaded (default is 1.3.35)
- GM_SOURCE=x provide an alternative (relative) filepath to a GraphicsMagick archive which you have downlaoded elsewhere (default is /External/download/GraphicsMagick-${GM_VERSION}.tar.gz - where ${GM_VERSION} is the version above)

eg: `cmake -DGM_VERSION=1.3.34 ..`

`make`

This will produce a 'jk' executable within the Build folder wich can be ran with `./jk` from within /Build.

`make install`  - this may require sudo

This will install the joker libraries and executable to /usr/local. Joker can now be invoked with `jk` in terminal.



## Usage & Performance 

To launch the joker welcome screen and view available commands use

`jk`

This will show the available flags which can be used to control training and OCR. Visit the [Wiki](https://github.com/jnoxro/joker/wiki) for further launch information

Current tests on an Intel® Pentium(R) CPU N3710 @ 1.60GHz × 4 processor result in a single threaded OCR time of <200 microseconds (0.2 milliseconds, 0.0002 seconds) for a 40*40pixel library size with 26 characters using the PixelAveraging OCR method.
