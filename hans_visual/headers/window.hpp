#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#include "_options.hpp"

class window_manager
{
    char test[21];

public:
     window_manager();
    ~window_manager();

    // Windowing ---------------

    GLFWwindow *window;             // The window object used to draw
    bool window_open;               // This is set True when the window is opened. User can set it False at will (useful in the main-loop)

    int open_GLFW_window();         // Open a window using GLFW
    void MakeContextCurrent();      // Makes the context of the specified window current for the calling thread
    void SwapBuffers_PollEvents();  // Swaps the front and back buffers of the specified window. Then, processes all pending events
    void Terminate();               // Terminates the GLFW library (destroys windows and cursors, frees allocated resources, restores modified gamma ramps)

    void GetFramebufferSize(int *display_w, int *display_h);

    // Controls ----------------

    void hide_cursor(bool hide);
};

#endif
