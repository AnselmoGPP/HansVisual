/*
*	TODO:
*		- Allow relative paths for calling LoadShaders()
*       - Cubes complete rotation (rotation method for a point)
*       - Parameters window (toolbar) (camera, transparency, axis...)
*       - Shaper with loop and without it
*       - Ordenar buffer para cubos para perfecta transparencia
*		-> Optimizations... Vextex in cubes are written at least 3 times. Optimize this. Also circular points, point sizes, line thickness...
*		-> Selection (or small windows for showing info about objects)
*		-> Are mutexes covering every situation?
*		>>> Crear varios visualizadores (cuidado con static variables) (comentar create_demo_windows) (controls camera static why?)
*       >>> Manipulate point size
*       >>> Manipulate window size
*       >>> Defaults: Screen size, point size, background color
*       >>> Eliminate <dequeu>
*       >>> Transformation matrices members
*       >>> Controls is a controled member of visualizerClass
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "shader.hpp"
#include "controls.hpp"

// Store one xyz point (not used)
struct pnt3D {
    float x, y, z;

    pnt3D(float a = 0, float b = 0, float c = 0) : x(a), y(b), z(c) { }

    pnt3D newData(float a, float b, float c) {
        x = a; y = b; z = c;
        return *this;
    }
};

// Store the data about the number of layers, their names and their capacity
struct layer_system {

	layer_system(
		unsigned int point_lay = 0, std::string *point_names = nullptr, unsigned int *max_pnt = 0,
		unsigned int line_lay = 0, std::string *line_names = nullptr, unsigned int *max_lin = 0,
		unsigned int triangle_lay = 0, std::string *triangle_names = nullptr, unsigned int *max_tri = 0,
		unsigned int cube_lay = 0, std::string *cube_names = nullptr, unsigned int *max_cub = 0
	) :
		point_layers(point_lay), line_layers(line_lay), cube_layers(cube_lay), triangle_layers(triangle_lay) {

		// Save layers names and maximum size of each layer
		point_layers_names = std::vector<std::string>(point_lay);
		max_points = std::vector<unsigned int>(point_lay);
		for (int i = 0; i < point_layers; i++)
		{
			point_layers_names[i] = point_names[i];
			max_points[i] = max_pnt[i];
		}

		line_layers_names = std::vector<std::string>(line_lay);
		max_lines = std::vector<unsigned int>(line_lay);
		for (int i = 0; i < line_layers; i++)
		{
			line_layers_names[i] = line_names[i];
			max_lines[i] = max_lin[i];
		}

		triangle_layers_names = std::vector<std::string>(triangle_lay);
		max_triangles = std::vector<unsigned int>(triangle_lay);
		for (int i = 0; i < triangle_layers; i++)
		{
			triangle_layers_names[i] = triangle_names[i];
			max_triangles[i] = max_tri[i];
		}

		cube_layers_names = std::vector<std::string>(cube_lay);
		max_cubes = std::vector<unsigned int>(cube_lay);
		for (int i = 0; i < cube_layers; i++)
		{
			cube_layers_names[i] = cube_names[i];
			max_cubes[i] = max_cub[i];
		}
	}

	layer_system(const layer_system &obj) {

		point_layers = obj.point_layers;
		line_layers = obj.line_layers;
		cube_layers = obj.cube_layers;
		triangle_layers = obj.triangle_layers;


		point_layers_names = std::vector<std::string>(point_layers);
		for (int i = 0; i < point_layers; i++)
			point_layers_names[i] = obj.point_layers_names[i];

		line_layers_names = std::vector<std::string>(line_layers);
		for (int i = 0; i < line_layers; i++)
			line_layers_names[i] = obj.line_layers_names[i];

		triangle_layers_names = std::vector<std::string>(triangle_layers);
		for (int i = 0; i < triangle_layers; i++)
			triangle_layers_names[i] = obj.triangle_layers_names[i];

		cube_layers_names = std::vector<std::string>(cube_layers);
		for (int i = 0; i < cube_layers; i++)
			cube_layers_names[i] = obj.cube_layers_names[i];


		max_points = std::vector<unsigned int>(point_layers);
		for (int i = 0; i < point_layers; i++)
			max_points[i] = obj.max_points[i];

		max_lines = std::vector<unsigned int>(line_layers);
		for (int i = 0; i < line_layers; i++)
			max_lines[i] = obj.max_lines[i];

		max_triangles = std::vector<unsigned int>(triangle_layers);
		for (int i = 0; i < triangle_layers; i++)
			max_triangles[i] = obj.max_triangles[i];

		max_cubes = std::vector<unsigned int>(cube_layers);
		for (int i = 0; i < cube_layers; i++)
			max_cubes[i] = obj.max_cubes[i];
	}

	layer_system& operator=(const layer_system &obj) {

		point_layers = obj.point_layers;
		line_layers = obj.line_layers;
		cube_layers = obj.cube_layers;
		triangle_layers = obj.triangle_layers;


		point_layers_names = std::vector<std::string>(point_layers);
		for (int i = 0; i < point_layers; i++)
			point_layers_names[i] = obj.point_layers_names[i];

		line_layers_names = std::vector<std::string>(line_layers);
		for (int i = 0; i < line_layers; i++)
			line_layers_names[i] = obj.line_layers_names[i];

		triangle_layers_names = std::vector<std::string>(triangle_layers);
		for (int i = 0; i < triangle_layers; i++)
			triangle_layers_names[i] = obj.triangle_layers_names[i];

		cube_layers_names = std::vector<std::string>(cube_layers);
		for (int i = 0; i < cube_layers; i++)
			cube_layers_names[i] = obj.cube_layers_names[i];


		max_points = std::vector<unsigned int>(point_layers);
		for (int i = 0; i < point_layers; i++)
			max_points[i] = obj.max_points[i];

		max_lines = std::vector<unsigned int>(line_layers);
		for (int i = 0; i < line_layers; i++)
			max_lines[i] = obj.max_lines[i];

		max_triangles = std::vector<unsigned int>(triangle_layers);
		for (int i = 0; i < triangle_layers; i++)
			max_triangles[i] = obj.max_triangles[i];

		max_cubes = std::vector<unsigned int>(cube_layers);
		for (int i = 0; i < cube_layers; i++)
			max_cubes[i] = obj.max_cubes[i];


		return *this;
	}

	unsigned int point_layers;
	std::vector<std::string> point_layers_names;
	std::vector<unsigned int> max_points;

	unsigned int line_layers;
	std::vector<std::string> line_layers_names;
	std::vector<unsigned int> max_lines;

	unsigned int cube_layers;
	std::vector<std::string> cube_layers_names;
	std::vector<unsigned int> max_cubes;

	unsigned int triangle_layers;
	std::vector<std::string> triangle_layers_names;
	std::vector<unsigned int> max_triangles;
};

// Structure used for sending cubes to the visualizer
struct cube3D {
	float X, Y, Z, width, height, length, rot_H;      // rotation in radians

	// Parameters: x, y, z (cube's center), width, height, length, rot_H (horizontal rotation)
	cube3D(float x, float y, float z, float w, float h, float l, float rh) :
		X(x), Y(y), Z(z), width(w), height(h), length(l), rot_H(rh) { }
};

// Names used for selecting different buffers
enum object_type { points, lines, triangles, cubes };

// Define the kind of the additional array you send
enum data_buffer{
    colors,             // Define colors of each point
    categories,         // Define category of each point. If number of categories > colors in the palette, the visualizer starts again from the beginning of the pallete
    gradient            // Define a gradient for each point that goes from a minimum to a maximum value. You enter the absolute minimum and maximum.
};

// ----------------------------------------------------------------------------

class visualizerClass {

    //controls &cam{ camera };
    controls cam;

	GLFWwindow* window;			// The window object used to draw
    int display_w, display_h;

	int run_thread();			// The thread where the visualizer is run

	layer_system layer_data;
	bool get_layer_data(std::string layer_name, object_type obj_type, unsigned int &layer, unsigned int &max_points);

	void HSVtoRGB(int H, double S, double V, float coloroutput[3]);

    size_t num_objects = 4;					// Points, lines, triangles, cubes
    char test[21];

    // GUI rendering
    void create_windows();
    void create_demo_windows();
    void change_alpha_channel(object_type object, std::string layer_name);		// Change the alpha values from the corresponding color buffer: 1(points), 2(lines), 3(boxes)

    // GUI state
	bool show_checkboxes = 1;
	bool show_data = 0;
	bool show_options = 0;
	std::string *data_window;
	int data_window_size;
	std::mutex mut_fill_data;
	float backg_color[3] = { 0.5, 0.5, 0.5 };
    float point_siz = 100;

    // Transformation matrices
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;
    glm::mat4 MVP;

    // Common buffers
    std::vector<std::vector<size_t>> objects_to_print;      // Number of objects that are going to be printed per layer
    std::vector<std::vector<size_t>> palette_sizes;         // Sizes of each palette (number of colors)
    std::vector<std::vector<float>> alpha_channels;

    std::vector<std::mutex*> mut;
	std::vector<bool*> checkboxes_values;                   // enum -> 0 (points), 1 (lines), 2 (triangles), 3 (cubes)

	std::vector<std::vector<float(*)[3]>> palettes;         // Palettes of colors for each layer

    const size_t default_palette_size = 21;
    float (*default_palette)[3];

	// Points data
	std::vector<float(*)[3]> point_buffers;			// Stores all the coordinates of all the points per layer
	std::vector<float(*)[4]> point_color_buffers;	// Stores the RGBA colors for each point

	// Lines data
	std::vector<float(*)[2][3]> line_buffers;
	std::vector<float(*)[2][4]> line_color_buffers;

    // Triangles
    std::vector<float(*)[3][3]> triangle_buffers;
    std::vector<float(*)[3][4]> triangle_color_buffers;

	// Cubes data
    std::vector<float(*)[12*3][3]> cube_buffers;
    std::vector<float(*)[12*3][4]> cube_color_buffers;

	// Parameters: X, Y (cube's center), x, y (point), rot (radians). It considers x as OpenGL's x, and y as OpenGL's -z.
    void rotation_H(float &x, float &y, float X, float Y, float rot);

    // In-main-loop functions for loading data to the GPU
    void load_points(GLuint *vertexbuffIDs, GLuint *colorbuffIDs);
    void load_lines(GLuint *linebuffIDs, GLuint *linecolorbuffIDs);
    void load_triangles(GLuint *trianglebuffIDs, GLuint *trianglecolorbuffIDs);
    void load_cubes(GLuint *cubebuffIDs, GLuint *cubecolorbuffIDs);

public:
	visualizerClass(
		 unsigned int point_lay = 0,		std::string *point_names = nullptr,		unsigned int *max_points = 0,
		 unsigned int line_lay = 0,			std::string *line_names = nullptr,		unsigned int *max_lines = 0,
		 unsigned int cubes_lay = 0,		std::string *cube_names = nullptr,		unsigned int *max_cubes = 0,
		 unsigned int triangles_lay = 0,	std::string *triangle_names = nullptr,	unsigned int *max_triangles = 0
	 );
    ~visualizerClass();

	visualizerClass& operator=(const visualizerClass &obj);

    // Main methods ---------------------------------

	// Create a window and open a new thread that runs the visualizer
    int open_window();

	// Add a new layer. Indicate the element type (points, lines, triangles, cubes), the name and the capacity (number of elements that layer supports)
	void add_layer(object_type type, const char *name, unsigned int capacity);

	// Send points to print. Optionally, include an additional array with the category of each point (cluster) or the color of each point, and specify what type of array it is:
    // - 'categories': Should be numbers starting from 0 to the number of categories - 1. When don't specified, the additional array is considered 'categories'.
    // - 'gradient': Must include the minimum and maximum values (inclusive). If not specified, the minimum is 0 and the maximum is 1.
    // - 'colors'
	// For more info, look at the macros definition (visualizerClass.hpp).
	// When no category or color array is sent, points are black.
    void send_points(std::string layer_name, int number_points, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

	// Send an array containing the points coordinates you want to bind with lines, and the number of points you want to use, including the gap-points with coordinates (1.2, 3.4, 5.6) (include this number in the number of points).
    void send_lines(std::string layer_name, int number_points, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    void send_triangles(std::string layer_name, int number_triangles, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Send array of cubes (cube3D) to print them
    void send_cubes(std::string layer_name, int number_cubes, const cube3D *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Bonus methods --------------------------------

	// Transform the coordinates of an array of points from automotive system (x:front, y:left, z:up) to OpenGL system (x:right, y:up, z:back).
	void transform_coordinates(float *points_arr, int number_points);

    // Send a new palette of colors to replace the current one. Each RGB value is in the range 0 - 1.
    void send_palette_RGB_01(std::string layer_name, object_type obj, float *new_palette, int number_colors);

	// Send a new palette of colors to replace the current one. Each RGB value is in the range 0 - 255.
	void send_palette_RGB(std::string layer_name, object_type obj, float *new_palette, int number_colors);

	// Send a new palette of colors to replace the current one. H(Hue): 0 - 360 degree (integer), *S(Saturation) : 0 - 1.00 (double), * V(Value) : 0 - 1.00 (double).
	void send_palette_HSV(std::string layer_name, object_type obj, float *new_palette, int number_colors);

	// Transform an array of HSV colors to RGB colors
	void convert_HSVtoRGB(float *colors, int num_colors);

	// Transform an array of RGB 0-255 colors to RGB 0-1 colors
	void convert_RGB255toRGB(float *colors, int num_colors);

    // Public GUI method. Publish data in the "data window". Send a pointer to an array of 10 std::strings. The empty strings (="") won't be published.
    void fill_data_window(const std::string *data_strings, int num_strings);

    // This represents the function [ y(x) = a + bx + cx^2 + dx^3 ]. It outputs the y value for the inclusive range [xmin, xmax] (including extremes). The sample variable is the number of segments (the result_array will store 'sample' + 1 elements)
    void pol_3th_degree(float *results_array, float xmin, float xmax, float sample, float a, float b, float c, float d);

	// Pass a pointer to an array[12][3] to store the icosahedron vertices. You must provide the radius too.
	void icosahedron(float side_length, float(*points)[3]);
};
