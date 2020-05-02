
#include "controls.hpp"

keys_controller * keys_controller::controls = nullptr;

keys_controller * keys_controller::get_instance()
{
    if (!controls)
        controls = new keys_controller();

    return controls;
}

void keys_controller::update_key_states(GLFWwindow *window)
{
    // Get cursor position  -  Cursor position given by the mouse: Origin: top-left, X: to the right, Y: downwards
    xpos0 = xpos;
    ypos0 = ypos;
    glfwGetCursorPos(window, &xpos, &ypos);     // Another option using callback:	glfwSetCursorPosCallback(window, cursorPositionCallback);

    // Get mouse buttons states
    R_just_released = false;
    R_first_press = R_pressed;
    glfwSetMouseButtonCallback(window, mouseButtonCallback);    // The callback function is run only when the mouse left/middle/right-button is pressed or released
    (R_pressed && !R_first_press) ? R_first_press = true : R_first_press = false;
    if (R_first_press) { sel_xpos0 = xpos; sel_ypos0 = ypos; };

    // Scroll roll
    scroll_yOffset = 0;
    glfwSetScrollCallback(window, scrollCallback);              // The callback function is run only when the scroll is rolled

    // Keys states
    (glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS)  ? up_press = true       : up_press = false;
    (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS)  ? down_press = true     : down_press = false;
    (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)  ? right_press = true    : right_press = false;
    (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS)  ? left_press = true     : left_press = false;
    (glfwGetKey(window, GLFW_KEY_W)     == GLFW_PRESS)  ? w_press = true        : w_press = false;
    (glfwGetKey(window, GLFW_KEY_S)     == GLFW_PRESS)  ? s_press = true        : s_press = false;
    (glfwGetKey(window, GLFW_KEY_D)     == GLFW_PRESS)  ? d_press = true        : d_press = false;
    (glfwGetKey(window, GLFW_KEY_A)     == GLFW_PRESS)  ? a_press = true        : a_press = false;

    if(cursor_pos[0] != 0 || cursor_pos[1] != 0)
    {
        glfwSetCursorPos(window, cursor_pos[0], cursor_pos[1]);
        cursor_pos[0] = 0;
        cursor_pos[1] = 0;
    }

    if(poll_events) glfwPollEvents();
}

bool keys_controller::setCursorPosInNextKeyUpdate(int width, int height)
{
    cursor_pos[0] = width;
    cursor_pos[1] = height;
}

// Callback functions ----------------------------

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    keys_controller * control = keys_controller::get_instance();

    glfwGetCursorPos(window, &control->xpos, &control->ypos);

    if		(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        control->L_pressed = false;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        control->L_pressed = true;
        control->ypos0 = control->ypos;
        control->xpos0 = control->xpos;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        control->R_pressed = false;
        control->R_just_released = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        control->R_pressed = true;
        control->sel_ypos0 = control->ypos;
        control->sel_xpos0 = control->xpos;
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        control->scroll_pressed = false;
    }
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        control->scroll_pressed = true;
        control->sel_ypos0 = control->ypos;
        control->sel_xpos0 = control->xpos;
    }
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    keys_controller::get_instance()->scroll_yOffset = yOffset;
}
