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

//#include "btBulletCollisionCommon.h"
//#include "btBulletDynamicsCommon.h"

#include "_options.hpp"
#include "window.hpp"
#include "layer.hpp"
#include "shader.hpp"
#include "selection.hpp"
#include "camera.hpp"
#include "gui.hpp"
#include "controls.hpp"

class plotter
{
    std::vector<layer> *layersSet;
    std::mutex *layersSet_mut;

    camera cam;
    my_gui gui;
    window_manager win;
    keys_controller *kc;
    int display_w, display_h;

    int main_loop_thread();		// The thread where the visualizer is run

    float backg_color[3];
    float point_siz;

    // Transformation matrices
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;
    glm::mat4 MVP;

    // In-main-loop functions for loading data to the GPU
    void load_buffers(std::map<char*, unsigned int> program);
    void load_buffer_3D(layer *lay, GLuint vertexbuffID, GLuint colorbuffID, unsigned int program);
    void load_buffer_2D(layer *lay, GLuint vertexbuffID, GLuint colorbuffID, unsigned int program);

    // Frequency (FPS)
    std::chrono::high_resolution_clock::time_point time_1, time_2;
    void fps_control(unsigned int frequency);           // Tell how many fps you want. If they are higher, they will be reduced until the specified fps

    // ID buffers
    void create_VAOs(int amount);
    GLuint *VertexArraysID;
    void create_VBOs(int amount, std::mutex *mut);
    GLuint *vertexbuffersIDs;
    GLuint *colorbuffersIDs;

    std::map<std::string, GLuint> unif = { {"MVP", 0}, {"Cam_pos", 0}, {"Pnt_size", 0} };
    void create_uniforms(GLuint programID);
    void send_uniforms();

    int check_glew(int result);
    void set_gl_options();
    void gl_static_draw_example();      // Just for illustration purposes
    void set_viewport_and_background();

    // Individual layer modifiers
    selection_square sqr_sel;
    points_selection pnt_sel;

public:
    plotter(std::vector<layer> *layers_set, std::mutex *layers_set_mutex);
    plotter(const plotter &obj);
    plotter& operator=(const plotter &obj);
    ~plotter();

    unsigned int frame_count = 0;

    // Main methods ---------------------------------

	// Create a window and open a new thread that runs the visualizer
    int open_window();

    // Resize the buffer set        (Call this just before resizing layersSet) (Use layersSet.mutex have thread under control)
    void resize_buffer_set(size_t new_size);

    // Check whether the window is open
    bool window_is_open();

    // Close the window
    void close_window();

    void fill_data_window(const std::string *data_strings, int num_strings);
};

#endif
