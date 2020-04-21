#ifndef HEADER_HPP
#define HEADER_HPP

#include "plotter.hpp"
#include "HansVisual.hpp"
#include "toolKit.hpp"

// Point buffers
extern float pnts[12][3];                           // Contains 12 points that form and icosahedron
extern unsigned int points_categories[12];
extern float points_colors_HSV[12][3];
extern float points_colors_HSV_2[12][3];
extern float points_colors_RGB[12][3];
extern float modified_rainbow[256][3];
extern float points_gradient_palette[21][3];
extern float points_gradients[12];
extern float points_line[256][3];
extern std::string points_names[12];

// Line buffers
extern float linesCube[12][2][3];
extern float parable[100][2][3];
extern unsigned int lines_categories[12][2];
extern float lines_colors[12][2][3];
extern float parable_gradient_palette[101][3];
extern float parable_gradients[100][2];

// Triangle buffers
extern float triangs[4][3][3];
extern unsigned int triangles_categories[4][3];
extern float triangles_colors[4][3][3];
extern float triangles_gradient_palette[11][3];
extern float triangles_gradients[4];

// Cube buffers
extern cube3D myCubes[3];
extern unsigned int cubes_categories[3];
extern float cubes_colors[3][3];
extern float cubes_gradient_palette[21][3];
extern float cubes_gradients[3];

// Function that fills some buffers. It uses some special functions from HansVisual
void fill_buffers();

#endif
