#ifndef TOOLKIT_HPP
#define TOOLKIT_HPP

#include <iostream>
#include <cmath>

namespace toolKit
{

// Auxiliar functions -----------------------------------------------------------

    void polynomial_x(float *results, float x, float *coefficients, float number_of_coefficients);

    void HSVtoRGB(double H, double S, double V, float output[3]);

    void invert_array(float (*arr)[3], size_t siz);

// Main functions ---------------------------------------------------------------

    void convert_HSVtoRGB(float (*colors)[3], int num_colors);

    // Transform coordinates of points in an array from X-first to OGL system
    void transform_coordinates(float (*points_arr)[3], int number_points);

    // Given some X values for a polynomial, get the points (float results[sample_size+1][3]) (Z=0) (sample_size = number of segments). Pass the minimum and maximum x values, and the number of x values to take inside that range (sample_size) (1 more value will be added at the end). Pass the coefficients of the polynomial (this determines its grade)
    void polynomial_points(float (*result)[3], float min_x, float max_x, unsigned int sample_size, float *coefficients, unsigned int number_of_coefficients);

    // Given some X values for a polynomial, get the segments (float results[sample_size][2][3]) (Z=0) (sample_size = number of segments). Pass the minimum and maximum x values, and the number of x values to take inside that range (sample_size). Pass the coefficients of the polynomial (this determines its grade)
    void polynomial_segments(float (*result)[2][3], float min_x, float max_x, unsigned int sample_size, float *coefficients, unsigned int number_of_coefficients);

    // Fill an array[12][3] with points forming an icosahedron
    void icosahedron(float side_length, float(*points)[3]);

    // Fill an array[256][3] with a rainbow palette. Define the Hue range in parameters min and max. Define if you want to invert positions of colors. And you can apply a deformation to the rainbow by taking some point in the spectrum and moving it to another place.lue to red.
    void fill_rainbow(float (*modified_rainbow)[3], float min = 0, float max = 240, bool inverted = true, float original_lvl = 180, float new_lvl = 100);

    // Create polyline: Given X number of points (arr[X][3]), fill an array (arr[X-1][2][3]) with a set of segments connecting those points, ready for sending to the visualizer
    void fill_polyline(float (*points)[3], size_t num_points, float (*result)[2][3]);

    // Get a vector with the points

}

#endif
