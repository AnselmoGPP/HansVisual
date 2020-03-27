#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

//#include "btBulletCollisionCommon.h"
//#include "btBulletDynamicsCommon.h"

#include "_options.hpp"
#include "window.hpp"
#include "layer.hpp"
#include "shader.hpp"
#include "camera.hpp"

class plotter
{
    std::vector<layer> *layersSet;
    std::mutex *layersSet_mut;

    camera cam;

    window_manager win;
    int display_w, display_h;

    int main_loop_thread();		// The thread where the visualizer is run

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

    // ID buffers
    void create_VAOs(int amount);
    GLuint *VertexArraysID;
    void create_VBOs(int amount, std::mutex *mut);
    GLuint *vertexbuffersIDs;
    GLuint *colorbuffersIDs;
    GLuint *selectionSquareID;
    GLuint *selectionSquareColorID;

    std::map<std::string, GLuint> unif = { {"MVP", 0}, {"Cam_pos", 0}, {"Pnt_size", 0} };
    void create_uniforms(GLuint programID);

    int check_glew(int result);
    void set_gl_options();
    void gl_static_draw_example();      // Just for illustration purposes

public:
    plotter(std::vector<layer> *layers_set, std::mutex *layers_set_mutex);
    plotter(const plotter &obj);
    plotter& operator=(const plotter &obj);
    ~plotter();


    // Main methods ---------------------------------

	// Create a window and open a new thread that runs the visualizer
    int open_window();

    // Public GUI method. Publish data in the "data window". Send a pointer to an array of 10 std::strings. The empty strings (="") won't be published.
    void fill_data_window(const std::string *data_strings, int num_strings);

    // Resize the buffer set        (Call this just before resizing layersSet) (Use layersSet.mutex have thread under control)
    void resize_buffer_set(size_t new_size);

    // Check whether the window is open
    bool window_is_open();

    // Close the window
    void close_window();
};

#endif
