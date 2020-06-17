#ifndef WINDOW_HPP
#define WINDOW_HPP

/*
    open_GLFW_window();
    MakeContextCurrent();
    glewExperimental = true;        // Needed for core profile
    if (glewInit() != GLEW_OK)      // Create the OpenGL context for the GLFW window
    {
        fprintf(stderr, "Failed to initialize GLEW\n"); getchar();
        Terminate();
        return -1;
    }

    do
    {
        if(!window_open) break;
        SwapBuffers_PollEvents();
    }
    while(scape_condition())

    Terminate();
*/

#include <GLFW/glfw3.h>

class window_manager
{
public:
     window_manager();
     window_manager(const window_manager &obj);
     window_manager & operator=(const window_manager &obj);
    ~window_manager();

    // Windowing ---------------

    GLFWwindow *window;             // The window object used to draw
    bool window_open;               // Set true when the window is opened. User can set it False at will (useful in the main-loop for closing the window)
    int display_w, display_h;       // Window size in pixels (width, height)

    int  open_GLFW_window();        // Open a window using GLFW
    void MakeContextCurrent();      // Makes the context of the specified window current for the calling thread
    void SwapBuffers_PollEvents();  // Swaps the front and back buffers of the specified window. Then, processes all pending events
    void Terminate();               // Terminates the GLFW library (destroys windows and cursors, frees allocated resources, restores modified gamma ramps)
    bool scape_conditions();

    void GetFramebufferSize();

private:
    char test[21];
};

#endif
