#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <GLFW/glfw3.h>

#include <iostream>

#include "_options.hpp"

// Callback functions: Get mouse click - Get mouse out/in window - Get cursor position
static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
static void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);

class keys_controller           // Singleton class
{
    static keys_controller *controls;

protected:

    keys_controller() = default;

public:

    static keys_controller *get_instance();

    void update_key_states(GLFWwindow *window);

    // Cursor position
    double xpos, ypos, xpos0, ypos0;
    //double sel_xpos, sel_ypos;        // <<<<<<< NOT NECESSARY
    double sel_xpos0, sel_ypos0;

    // Mouse
    bool L_pressed, R_pressed, scroll_pressed;
    bool R_first_press, R_just_released = false;
    double scroll_yOffset;

    // Keys
    bool up_press, down_press, right_press, left_press;
    bool w_press, s_press, d_press, a_press;

};

#endif
