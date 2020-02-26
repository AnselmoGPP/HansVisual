#ifndef TOOLKIT_HPP
#define TOOLKIT_HPP

#include <iostream>
#include <cmath>

namespace toolKit
{
// Auxiliar functions -----------------------------------------------------------

    extern void polynomial_x(float *results, float x, float *coefficients, float number_of_coefficients);

    extern void HSVtoRGB(int H, double S, double V, float output[3]);

    extern void invert_array(float (*arr)[3], size_t siz);

// Main functions ---------------------------------------------------------------

    extern void convert_HSVtoRGB(float *colors, int num_colors);

    // Transform coordinates of points in an array from X-first to OGL system
    extern void transform_coordinates(float *points_arr, int number_points);

    // Given some X values for a polynomial, get the Y values. Pass the minimum and maximum x values, and the number of x values to take inside that range (sample_size) (1 will be added at the end). Pass the coefficients of the polynomial (this determines its grade)
    extern void polynomial_graph(float (*result)[3], float min_x, float max_x, int sample_size, float *coefficients, float number_of_coefficients);

    // Fill an array[12][3] with points forming an icosahedron
    extern void icosahedron(float side_length, float(*points)[3]);

    // Fill an array[256][3] with a rainbow palette. Define the Hue range in parameters min and max. Define if you want to invert positions of colors. And you can apply a deformation to the rainbow by taking some point in the spectrum and moving it to another place.lue to red.
    extern void fill_rainbow(float (*modified_rainbow)[3], float min = 0, float max = 240, bool inverted = true, float original_lvl = 180, float new_lvl = 100);

    // Get a vector with the points

}

#endif
