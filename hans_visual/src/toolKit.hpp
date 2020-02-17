#ifndef TOOLKIT_HPP
#define TOOLKIT_HPP

#include "layers.hpp"

namespace toolKit
{
    static void convert_HSVtoRGB(float *colors, int num_colors)
    {
        layer obj;
        obj.convert_HSVtoRGB(colors, num_colors);
    }

    static void convert_RGB255toRGB(float *colors, int num_colors)
    {
        layer obj;
        obj.convert_RGB255toRGB(colors, num_colors);
    }

    static void transform_coordinates(float *points_arr, int number_points)
    {
        layer obj;
        obj.transform_coordinates(points_arr, number_points);
    }

    static void polynomial_graph(float (*result)[3], float min_x, float max_x, int sample_size, float *coefficients, float number_of_coefficients)
    {
        layer obj;
        obj.polynomial_graph(result, min_x, max_x, sample_size, coefficients, number_of_coefficients);
    }

    static void icosahedron(float side_length, float(*points)[3])
    {
        layer obj;
        obj.icosahedron(side_length, points);
    }
}

#endif
