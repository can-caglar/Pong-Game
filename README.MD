# Pong Game Project

*Version 1.0.0*

A classic game of Pong implemented using the SDL2 library. 

This project is a submission for the Udacity C++ Nanodegree program capstone project. No template code from Udacity was used.

---
## Expected Behavior

<img src="images/demo.gif"/>

## Dependencies for Running Locally

* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* SDL_Image >= 2.0
	* All installation instructions can be found [here](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php). Download link can be found [here](
	https://www.libsdl.org/tmp/SDL_image/) is the official site.
* SDL_ttf >= 2.0
	* Follow the same installation procedure as SDL_Image. Download link to library is [here](https://www.libsdl.org/projects/SDL_ttf/).
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)


## Basic Build Instructions

1. Clone this repo.
2. Enter directory: `cd Pong-Game`
3. Make a build directory in the top level directory: `mkdir build && cd build`
4. Compile: `cmake .. && make`
5. Copy resources folder to executable folder `cp -r ../FP/Resources .`
6. Run it: `./PongByCan`


## Gameplay instructions

Player 1 controls : W/D
Player 2 controls : UP/DOWN
Press SPACE to start the game.

## File Class Structure

<img src="images/pong_class_diagram.png"/>

## Rubric points addressed

* The project demonstrates an understanding of C++ functions and control structures.
	* Game.cpp, line 267
* The project uses Object Oriented Programming techniques.
	* GameObjects.h
* Classes use appropriate access specifiers for class members.
	* GameObjects.h
* Classes abstract implementation details from their interfaces.
	* Game.h
* The project follows the Rule of 5.
	* GameObjects.h line 36-40
* The project uses smart pointers instead of raw pointers.
	* Game.h line 46-48
* The project uses multithreading.
	* Timer.cpp line 34
* A mutex or lock is used in the project.
	* Timer.cpp line 20
