#ifndef LAYERS_HPP
#define LAYERS_HPP

#include <iostream>
#include <vector>
#include <mutex>
#include <cmath>

#include "_options.hpp"

// Store one xyz point (not used)
struct pnt3D {
    float x, y, z;

    pnt3D(float a = 0, float b = 0, float c = 0);

    pnt3D newData(float a, float b, float c);
};

// Structure used for sending cubes to the visualizer
struct cube3D {
    float X, Y, Z, width, height, length, rot_H;      // rotation in radians

    // Parameters: x, y, z (cube's center), width, height, length, rot_H (horizontal rotation)
    cube3D(float x, float y, float z, float w, float h, float l, float rh);
};

// Names used for selecting different buffers
enum object_type { points, lines, triangles, cubes, none };

// Used to allow only one layer edition per main-loop iteration
enum layer_state{ open, closed };

struct layer
{
    layer();
    layer(const char *name, object_type type, unsigned int capacity);
    layer(const layer &obj);
    layer& operator=(const layer &obj);
    ~layer();

    // Main layer data
    std::string layer_name;
    unsigned int max_objs;
    object_type layer_type;
    size_t objs_to_print = 0;                    // Number of objects that are going to be printed
    layer_state state = open;                    // Used for only modifying buffer once per loop
    bool checkbox_visible = true;
    bool checkbox_value = true;
    unsigned int dimensions = 3;
    std::mutex *layerMutex = nullptr;

    // Colors
    float(*palette)[3] = nullptr;                   // Palette of colors of the layer
    size_t palette_size = 21;                       // Size of the layer's palette (number of colors)
    float alpha_channel = 1.0f;

    // Send array of points (float arr[i]) to print them
    int send_points(unsigned int number_points, const float (*arr)[3], float R = DEFAULT_RED, float G = DEFAULT_GREEN, float B = DEFAULT_BLUE, std::string *points_data = nullptr);
    int send_points_categories(unsigned int number_points, const float (*arr)[3], const unsigned int *categories, std::string *points_data = nullptr );
    int send_points_colors(unsigned int number_points, const float (*arr)[3], const float (*colors)[3], std::string *points_data = nullptr );
    int send_points_gradients(unsigned int number_points, const float (*arr)[3], const float *gradients, float min = 0, float max = 1, std::string *points_data = nullptr );

    // Send an array containing pairs of vertex (float arr[i][2]), each pair representing the ends of a segment you want to draw
    int send_lines(unsigned int number_lines, const float (*arr)[2][3], float R = DEFAULT_RED, float G = DEFAULT_GREEN, float B = DEFAULT_BLUE);
    int send_lines_categories(unsigned int number_lines, const float (*arr)[2][3], const unsigned int (*categories)[2]);
    int send_lines_categories(unsigned int number_lines, const float (*arr)[2][3], const unsigned int  *categories);
    int send_lines_colors(unsigned int number_lines, const float (*arr)[2][3], const float (*colors)[2][3]);
    int send_lines_colors(unsigned int number_lines, const float (*arr)[2][3], const float (*colors)[3]);
    int send_lines_gradients(unsigned int number_lines, const float (*arr)[2][3], const float (*gradients)[2], float min = 0, float max = 1);
    int send_lines_gradients(unsigned int number_lines, const float (*arr)[2][3], const float  *gradients,     float min = 0, float max = 1);

    // Send array of triangles (float arr[i][3]) to print them
    int send_triangles(unsigned int number_triangles, const float (*arr)[3][3], float R = DEFAULT_RED, float G = DEFAULT_GREEN, float B = DEFAULT_BLUE);
    int send_triangles_categories(unsigned int number_triangles, const float (*arr)[3][3], const unsigned int (*categories)[3]);
    int send_triangles_categories(unsigned int number_triangles, const float (*arr)[3][3], const unsigned int  *categories);
    int send_triangles_colors(unsigned int number_triangles, const float (*arr)[3][3], const float (*colors)[3][3]);
    int send_triangles_colors(unsigned int number_triangles, const float (*arr)[3][3], const float (*colors)[3]);
    int send_triangles_gradients(unsigned int number_triangles, const float (*arr)[3][3], const float (*gradients)[3], float min = 0, float max = 1);
    int send_triangles_gradients(unsigned int number_triangles, const float (*arr)[3][3], const float  *gradients,     float min = 0, float max = 1);

    // Send array of cubes (cube3D arr[i]) to print them
    int send_cubes(unsigned int number_cubes, const cube3D *arr, float R = DEFAULT_RED, float G = DEFAULT_GREEN, float B = DEFAULT_BLUE);
    int send_cubes_categories(unsigned int number_cubes, const cube3D *arr, const unsigned int *categories);
    int send_cubes_colors(unsigned int number_cubes, const cube3D *arr, const float (*colors)[3]);
    int send_cubes_gradients(unsigned int number_cubes, const cube3D *arr, const float *gradients, float min = 0, float max = 1);

    // Change the layer's palette for another (3 available formats)
    int send_palette_RGB_01(float (*new_palette)[3], int number_colors);
    int send_palette_RGB(float (*new_palette)[3], int number_colors);
    int send_palette_HSV(float (*new_palette)[3], int number_colors);

    float* get_vertex_ptr();
    float* get_colors_ptr();
    int set_alpha_channel(float alpha_value);
    void change_name(std::string new_name);

    // Points data
    float(*points_buffer)[3]                = nullptr;      // Stores all the coordinates of all the points of the layer
    float(*points_color_buffer)[4]          = nullptr;      // Stores the RGBA colors of each point
    std::string *points_strings             = nullptr;      // Optional data for each point (used in selections)
    // Lines data
    float(*lines_buffer)[2][3]              = nullptr;
    float(*lines_color_buffer)[2][4]        = nullptr;
    // Triangles
    float(*triangles_buffer)[3][3]          = nullptr;
    float(*triangles_color_buffer)[3][4]    = nullptr;
    // Cubes data
    float(*cubes_buffer)[12*3][3]           = nullptr;
    float(*cubes_color_buffer)[12*3][4]     = nullptr;

private:

    void error_message(unsigned int code, unsigned int number_objs = 0);
    void rotation_H(float &x, float &y, float X, float Y, float rot);
    void HSVtoRGB(double H, double S, double V, float output[3]);
    int first_checks(object_type func_type, unsigned int number_elements);
    void fill_cube_vertex(const cube3D *arr);
    std::string layer_type_str();
};

#endif
