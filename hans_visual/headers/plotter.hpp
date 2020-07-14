#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <vector>
#include <string>
#include <mutex>
#include <chrono>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

//#include "btBulletCollisionCommon.h"
//#include "btBulletDynamicsCommon.h"

#include "window.hpp"
#include "selection.hpp"
#include "gui.hpp"
#include "layer.hpp"
#include "camera.hpp"
#include "controls.hpp"

// Functions for getting the path to the library
#if defined(__unix__)       //#ifdef __unix__   __unix   __USE_UNIX98
    // Do something OS-dependent
#elif _WIN32               // #ifdef defined(_WIN32) || defined(_WIN64)
    // Do something OS-dependent
#endif

class std_timer
{
public:
    long deltaTime;
    void get_delta_time();                      // Get time increment between two consecutives calls to this function
    void fps_control(unsigned int frequency);   // Argument: Desired FPS (a sleep will be activated to slow down the process)

private:
    bool first_call = true;
    std::chrono::high_resolution_clock::time_point lastTime, currentTime;
};

class plotter
{
public:
    plotter(std::vector<layer> *layers_set, std::mutex *layers_set_mutex);
    plotter(const plotter &obj);
    plotter& operator=(const plotter &obj);
    ~plotter();

    unsigned int frame_count;

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

    void getCamPosition(float *position);

private:
    std::vector<layer> *layersSet;
    std::mutex *layersSetMutex;

    cam_factory camF;
    my_gui gui;
    window_manager win;
    keys_controller *kc;
    std_timer timer;

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
    void load_buff(layer *lay, GLuint vertexbuffID, GLuint colorbuffID, unsigned int program);

    // ID buffers
    void create_VAOs(int amount);
    GLuint *VertexArraysID;
    void create_VBOs(int amount, std::mutex *layerSet_mutex);
    GLuint *vertexbuffersIDs;
    GLuint *colorbuffersIDs;

    std::map<std::string, GLuint> unif;
    void create_uniforms(GLuint programID);
    void send_uniforms();

    int check_glew(int result);
    void set_gl_options();
    void set_viewport_and_background();
    void gl_static_draw_example();      // Illustration purposes
    void print_data();                  // Debugging purposes

    // Individual layer modifiers
    selection_square sqr_sel;
    points_selection pnt_sel;
};

#endif
