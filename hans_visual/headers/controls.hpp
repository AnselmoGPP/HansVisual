#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <GLFW/glfw3.h>

// Callback functions: Get mouse click - Get mouse out/in window - Get cursor position
static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
static void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);

class keys_controller           // Singleton class
{
public:

    static keys_controller *get_instance();

    void sticky_keys(GLFWwindow *window, bool sticky);
    void update_key_states(GLFWwindow *window);
    bool setCursorPosInNextKeyUpdate(int width, int height);
    bool poll_events = false;

    // Cursor position
    double xpos = 0, ypos = 0, xpos0 = 0, ypos0 = 0;
    float sel_xpos0, sel_ypos0;

    // Mouse
    bool L_pressed, R_pressed, scroll_pressed;
    bool R_first_press, R_just_released = false;
    float scroll_yOffset;                           // <<< should be int?

    // Keyboard
    bool up_press, down_press, right_press, left_press;
    bool w_press, s_press, d_press, a_press;
    bool p_press = false, p_still_press = false;

protected:
    keys_controller() = default;

private:
    static keys_controller *controls;

    int cursor_pos[2] = {0, 0};
};

#endif
