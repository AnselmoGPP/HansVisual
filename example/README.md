# Shaper

This program is used as an example for making basic shapes/points/lines using the (static) library "visualizer_class", that uses OpenGL. This is a playground for low level 3D drawing. 
The shapes are made with functions saved in the "main.cpp" file and called by the main() function. Once a function is called, the OpenGL visualizer is opened for a certain amount of time (determined in main()) and you can explore your shape. 

Set the variable OP_SYSTEM from the CMakeLists.txt to Linux or Windows, depending on which operative system you will build and compile this program.

Functions included so far:

- void icosahedron(float side_length, float (*points)[3]): Draws the vertices of an icosahedron (black points).
