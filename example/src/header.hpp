#ifndef HEADER_HPP
#define HEADER_HPP

#include "HansVisual.hpp"
#include "toolKit.hpp"

// Point buffers
extern float pnts[12][3];                           // Contains 12 points that form and icosahedron
extern float points_categories[12];
extern float points_colors_HSV[12][3];
extern float points_colors_HSV_2[12][3];
extern float points_colors_RGB[12][3];
extern float points_gradient_palette[21][3];
extern float points_gradients[12];

extern float points_line[30][3];
extern float points_colors_line_HSV[30][3];
extern float points_line_2[256][3];
extern std::string points_names[12];

// Line buffers
extern float myLines[101 + 1 + 9][3];               // 2 lines + one line jump between them
extern float lines_categories[101 + 1 + 9 - 1];     // Category for each segment (number of segments = number of points - 1)
extern float lines_colors[101 + 1 + 9 - 1][3];      // Color for each segment (number of segments = number of points - 1)
extern float lines_gradient_palette[101][3];
extern float lines_gradients[101 + 1 + 9 - 1];

// Triangle buffers
extern float triangs[4][3][3];
extern float triangles_categories[4];
extern float triangles_colors[4][3];
extern float triangles_gradient_palette[11][3];
extern float triangles_gradients[4];

// Cube buffers
extern cube3D myCubes[3];
extern float cubes_categories[3];
extern float cubes_colors[3][3];
extern float cubes_gradient_palette[21][3];
extern float cubes_gradients[3];

// Function that fills some buffers. It uses some special functions from HansVisual
void fill_buffers();

#endif
