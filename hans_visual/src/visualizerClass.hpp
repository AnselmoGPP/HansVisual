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
*/

#define BACKG_R 0.
#define BACKG_G 0.
#define BACKG_B 0.14
#define PNT_SIZE 35

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

//#include "btBulletCollisionCommon.h"
//#include "btBulletDynamicsCommon.h"

#include "layers.hpp"
#include "shader.hpp"
#include "controls.hpp"

class visualizerClass {

    //controls &cam{ camera };
    controls cam;

	GLFWwindow* window;			// The window object used to draw
    int display_w, display_h;

	int run_thread();			// The thread where the visualizer is run	

    char test[21];

    // GUI rendering
    void create_windows();
    void create_demo_windows();

    // GUI state
	bool show_checkboxes = 1;
	bool show_data = 0;
	bool show_options = 0;
	std::string *data_window;
	int data_window_size;
	std::mutex mut_fill_data;
    float backg_color[3] = { BACKG_R, BACKG_G, BACKG_B };
    float point_siz = PNT_SIZE;

    // Transformation matrices
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;
    glm::mat4 MVP;

	// Points selection
    void check_selections();    // Points selection search ( http://www.3dkingdoms.com/selection.html ). Looks for the selected points and print its data, if an array of strings was provided
	double projmatrix[16];
	double mvmatrix[16];
	int viewport[4];
    double MinDistance = 0.01;                      // Selection distance: Minimum distance between the unitary pixel ray and the unitary point ray (direction vectors)
    void check_ray(double xpos, double ypos);		// Send a ray from a certain pixel and set the points near to the ray true in selected_points[]
    //std::vector<std::string> strings_to_show;       // Strings of selected points are stored here
    //void copy_selections_to_array();                // Copy the strings of selected points into strings_to_show[]
    float selection_color[4] = { 0.97, 1., 0., 1. };
    //float num_selected_points = 0;

    // Layers data
    //layer selections = layer("selections", points, 100);
    //int point_layers = 0, line_layers = 0, triangle_layers = 0, cube_layers = 0;

    // Common buffers
    std::vector<layer> layersSet;

    //const size_t default_palette_size = 21;
    //float (*default_palette)[3];

    //float default_color[3] = { 1., 1., 1. };

    // Selection data
    layer selection_square = layer("Sel. square", points, 0);
    //layer selected_points = layer("Selections", points, 0);
    std::vector<glm::vec3> temp_selections;

    // In-main-loop functions for loading data to the GPU
    void load_selectionSquare(GLuint *selectionitemsIDs, GLuint *selectioncolorsIDs);
    //void load_selected_points(GLuint *selectionSquareID, GLuint *selectionColorID);
    void load_buffers(GLuint *vertexbuffIDs, GLuint *colorbuffIDs);

    // Frequency (FPS)
    std::chrono::high_resolution_clock::time_point time_1, time_2;
    void fps_control(unsigned int frequency);           // Tell how many fps you want. If they are higher, they will be reduced until the specified fps

public:
    visualizerClass();
    visualizerClass(const visualizerClass &obj);
    visualizerClass& operator=(const visualizerClass &obj);
    ~visualizerClass();

    // Main methods ---------------------------------

	// Create a window and open a new thread that runs the visualizer
    int open_window();

	// Add a new layer. Indicate the element type (points, lines, triangles, cubes), the name and the capacity (number of elements that layer supports)
    void add_layer(const char *name, object_type type, unsigned int capacity);

    // Send array of points to print them
    void send_points(std::string layer_name, unsigned int number_points, const float *arr, const float *labels = nullptr, std::string *points_data = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

	// Send an array containing the points coordinates you want to bind with lines, and the number of points you want to use, including the gap-points with coordinates (1.2, 3.4, 5.6) (include this number in the number of points).
    void send_lines(std::string layer_name, unsigned int number_points, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Send array of triangles (sets of 3 points) to print them
    void send_triangles(std::string layer_name, unsigned int number_triangles, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Send array of cubes (cube3D) to print them
    void send_cubes(std::string layer_name, unsigned int number_cubes, const cube3D *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Bonus methods --------------------------------

	// Blue to red palette. Blue-yellow and yellow-red are in a different scale
	float modified_rainbow[256][3];

    // Transform the coordinates of an array of points from X-first system (x:front, y:left, z:up) to OpenGL system (x:right, y:up, z:back)
	void transform_coordinates(float *points_arr, int number_points);

    void send_palette_RGB_01(std::string layer_name, float *new_palette, int number_colors);

    void send_palette_RGB(std::string layer_name, float *new_palette, int number_colors);

    void send_palette_HSV(std::string layer_name, float *new_palette, int number_colors);

	// Transform an array of HSV colors to RGB colors
	void convert_HSVtoRGB(float *colors, int num_colors);

	// Transform an array of RGB 0-255 colors to RGB 0-1 colors
	void convert_RGB255toRGB(float *colors, int num_colors);

    // Public GUI method. Publish data in the "data window". Send a pointer to an array of 10 std::strings. The empty strings (="") won't be published.
    void fill_data_window(const std::string *data_strings, int num_strings);

    // This represents the function [ y(x) = a + bx + cx^2 + dx^3 ]. It outputs the y value for the inclusive range [xmin, xmax] (including extremes). The sample variable is the number of segments (the result_array will store 'sample' + 1 elements)
    void polynomial_graph(float (*result)[3], float min_x, float max_x, int sample_size, float *coefficients, float number_of_coefficients);

	// Pass a pointer to an array[12][3] to store the icosahedron vertices. You must provide the radius too.
	void icosahedron(float side_length, float(*points)[3]);
};
