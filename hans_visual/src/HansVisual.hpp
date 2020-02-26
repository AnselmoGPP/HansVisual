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

#ifndef HANSVISUAL_HPP
#define HANSVISUAL_HPP

#define BACKG_R 0.
#define BACKG_G 0.
#define BACKG_B 0.14
#define PNT_SIZE 35
#define MAX_PNT_SIZE 500.0f
#define DESIRED_FPS 60
#define ANTIALIASING 4      // x4 antialiasing
#define VERSION_GLFW 3
#define VERSION_GLFW_for_IMGUI "#version 330"
#define POLYGON_MODE 0      // Show only the borders of the triangles

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

#include "layer.hpp"
#include "shader.hpp"
#include "controls.hpp"

class HansVisual
{
    std::vector<layer> layersSet;

    controls cam;

    GLFWwindow* window;			// The window object used to draw
    int display_w, display_h;

    int main_loop_thread();		// The thread where the visualizer is run
    bool window_open;           // True if main_loop_thread is running. If false, it's out (or will get out asap)

    char test[21];

    // GUI rendering
    void create_windows();
    void create_demo_windows();

    // GUI state
	bool show_checkboxes = 1;
	bool show_data = 0;
	bool show_options = 0;
    int data_window_size;
	std::string *data_window;
	std::mutex mut_fill_data;
    float backg_color[3];
    float point_siz;

    // Transformation matrices
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;
    glm::mat4 MVP;

	// Points selection
    void check_selections();    // Points selection search ( http://www.3dkingdoms.com/selection.html ). Looks for the selected points and print its data, if an array of strings was provided
    void check_ray(double xpos, double ypos);		// Send a ray from a certain pixel and set the points near to the ray true in selected_points[]
    double projmatrix[16];
	double mvmatrix[16];
	int viewport[4];
    double MinDistance;                      // Selection distance: Minimum distance between the unitary pixel ray and the unitary point ray (direction vectors)
    float selection_color[4];
    layer selection_square;
    std::vector<glm::vec3> temp_selections;

    // In-main-loop functions for loading data to the GPU
    void load_selectionSquare(GLuint *selectionitemsIDs, GLuint *selectioncolorsIDs);
    void load_buffers(GLuint *vertexbuffIDs, GLuint *colorbuffIDs);

    // Frequency (FPS)
    std::chrono::high_resolution_clock::time_point time_1, time_2;
    void fps_control(unsigned int frequency);           // Tell how many fps you want. If they are higher, they will be reduced until the specified fps

public:
    HansVisual();
    HansVisual(const HansVisual &obj);
    HansVisual& operator=(const HansVisual &obj);
    ~HansVisual();

    // Main methods ---------------------------------

	// Create a window and open a new thread that runs the visualizer
    int open_window();

	// Add a new layer. Indicate the element type (points, lines, triangles, cubes), the name and the capacity (number of elements that layer supports)
    void add_layer(const char *name, object_type type, unsigned int capacity);

    // Send array of points to print them
    void send_points(std::string layer_name, unsigned int number_points, const float *arr, const float *labels = nullptr, std::string *points_data = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Send an array containing pairs of vertex, each pair representing the ends of a segment you want to draw
    void send_lines(std::string layer_name, unsigned int number_points, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Send array of triangles (sets of 3 points) to print them
    void send_triangles(std::string layer_name, unsigned int number_triangles, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Send array of cubes (cube3D) to print them
    void send_cubes(std::string layer_name, unsigned int number_cubes, const cube3D *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1);

    // Bonus methods --------------------------------

    void send_palette_RGB_01(std::string layer_name, float *new_palette, int number_colors);

    void send_palette_RGB(std::string layer_name, float *new_palette, int number_colors);

    void send_palette_HSV(std::string layer_name, float *new_palette, int number_colors);

    // Public GUI method. Publish data in the "data window". Send a pointer to an array of 10 std::strings. The empty strings (="") won't be published.
    void fill_data_window(const std::string *data_strings, int num_strings);

    // Clear the contents of a layer
    void clear_layer(std::string name);

    // Create a layer with a grid (or modify it if already exists). Parameters: Size of one cell's side, number of cells per side, and HSV color.
    void draw_grid(float cell_size, int grid_size, int H, double S, double V);

    // Check whether the window is open
    bool window_is_open();

    // Close the window
    void close_window();
};

#endif
