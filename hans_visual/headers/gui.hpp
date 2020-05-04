#ifndef GUI_HPP
#define GUI_HPP

/*
    init_gui();
    ImGui_ImplGlfw_InitForOpenGL(win.window, true);
    ImGui_ImplOpenGL3_Init(gui.glsl_version);

    do
    {
        if (!WantCaptureMouse()) camera::computeMatricesFromInputs();
        new_frame();
        create_gui_1();         // fill_data_window();  -->  This can be called at any time (uses mutex in these 2 functions)
        render_gui();
    }
    while(1)

    cleanup();
*/

#include <mutex>

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "_options.hpp"
#include "layer.hpp"

//class plotter;      // Defined in "plotter.hpp"

class my_gui
{
    ImGuiIO *io;        // Access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags)

    // States
    bool show_checkboxes = 1;
    bool show_data = 0;
    bool show_options = 0;

    void create_demo_windows();    // Demo window for illustration purposes

    // GUI 1 (visualizer)
    int data_window_size;
    std::string *data_window;
    std::mutex fillDataMutex;

public:

    my_gui();
    my_gui(const my_gui &obj);
    my_gui & operator=(const my_gui &obj);

    const char* glsl_version;

    void init_gui();        // Just after calling init_gui(), call ImGui_ImplGlfw_InitForOpenGL(win.window, true) and ImGui_ImplOpenGL3_Init(gui.glsl_version)
    void new_frame();
    void render_gui();
    void cleanup();

    bool WantCaptureMouse();

    // GUI 1 (visualizer)
    void create_gui_1(std::vector<layer> *layersSet, float *backg_color, float *point_siz);
    void fill_data_window(const std::string *data_strings, int num_strings);    // Publish data in the "data window". Send a pointer to an array of X std::strings. The empty strings (="") won't be published
};

#endif
