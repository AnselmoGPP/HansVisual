#include <stdio.h>
//#include <stdlib.h>

#include <GLFW/glfw3.h>

#include "_options.hpp"
#include "window.hpp"

window_manager::window_manager()
{
    window = nullptr;
    window_open = false;
    display_w = WINDOW_WIDTH;   display_h = WINDOW_HEIGHT;

    char test2[21] = { 0x3c, 0x3c, 0x3c, 0x20, 0x20, 0x48, 0x61,
                       0x6e, 0x73, 0x56, 0x69, 0x73, 0x75, 0x61,
                       0x6c, 0x20, 0x20, 0x3e, 0x3e, 0x3e, 0x00 };
    for(int i = 0; i < 21; i++) test[i] = test2[i];
}

window_manager::window_manager(const window_manager &obj)
{
    window = nullptr;
    window_open = false;
    display_w = obj.display_w;
    display_h = obj.display_h;

    for(size_t i = 0; i < 21; ++i) test[i] = obj.test[i];
}

window_manager & window_manager::operator=(const window_manager &obj)
{
    //window = nullptr;
    //window_open = false;
    display_w = obj.display_w;
    display_h = obj.display_h;

    for(size_t i = 0; i < 21; ++i) test[i] = obj.test[i];
}

window_manager::~window_manager() = default;

int window_manager::open_GLFW_window()
{
    // Initiate GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    // Set some options
    glfwWindowHint(GLFW_SAMPLES, ANTIALIASING);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_GLFW);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_GLFW);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window
    window = glfwCreateWindow(display_w, display_h, test, NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -2;
    }

    window_open = true;
    return 0;
}

void window_manager::MakeContextCurrent()
{
    glfwMakeContextCurrent(window);
}

void window_manager::SwapBuffers_PollEvents()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void window_manager::Terminate()
{
    glfwTerminate();
    window_open = false;
}

void window_manager::GetFramebufferSize()
{
    glfwGetFramebufferSize(window, &display_w, &display_h);
}

bool window_manager::scape_conditions()
{
    return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) && window_open;
}
