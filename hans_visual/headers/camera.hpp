#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>
#include <cmath>
#include <iomanip>
#include <mutex>

#include "_options.hpp"
#include "controls.hpp"

//class camera;
//extern camera *camHandler;
//extern std::mutex cam_mut;

class camera
{
    GLFWwindow *window;
    keys_controller * kc;

    double pi = 3.1415926536;

	// Time
	float lastTime = 0.0;
	float currentTime;
	float deltaTime;

	// Mouse
    //double xpos, ypos, xpos0, ypos0;
    //bool L_pressed, R_pressed, scroll_pressed;

	// camera parameters
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;
	float speed;
	float mouseSpeed;
	glm::vec3 sphere_center;
	float radius;
	float scroll_speed;
	float minimum_radius;
    float right_speed;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

    void get_angular_position();

    void computeMatricesFromInputs_FP(float aspect_ratio);
    void computeMatricesFromInputs_spherical(float aspect_ratio);

    enum cam_system{fp, sphere, plane, fps};

public:
    camera(int mode = CAM_MODE);
    //camera();
    //camera(GLFWwindow *win, int mode = CAM_MODE);
    //~camera() = default;

    glm::vec3 position;
    cam_system camera_mode;

    void associate_window(GLFWwindow *window_in);
    void computeMatricesFromInputs(float aspect_ratio);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void sticky_keys(bool sticky);
    void set_mouse_position_visibility();
    void hide_cursor(bool hide);

    //friend void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    //friend void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);

	// Points selection
    //double sel_xpos, sel_ypos, sel_xpos0, sel_ypos0;

    //bool R_just_released = false;
    //bool is_R_pressed() { return R_pressed; }
};

#endif
