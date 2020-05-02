#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>
#include <cmath>
//#include <iomanip>

#include "_options.hpp"
#include "controls.hpp"

class controls_timer
{
    float lastTime = 0.0;
    float currentTime;
public:
    float deltaTime;
    void get_delta_time();  // Get time increment between two consecutives calls to this function
};

enum cam_system{fp, sphere, plane, fps, zenith, sphere_z};

class camera
{
    GLFWwindow *window;
    keys_controller * kc;

    double pi = 3.1415926536;

    controls_timer timer;

	// camera parameters
    float horMove, vertMove;
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

    void get_cursor_displacement();

    void computeMatricesFromInputs_FP(float aspect_ratio);
    void computeMatricesFromInputs_spherical(float aspect_ratio);

public:
    camera(cam_system mode = (cam_system)CAM_MODE);

    glm::vec3 cam_position;
    cam_system camera_mode;

    void associate_window(GLFWwindow *window_in);
    void computeMatricesFromInputs(float aspect_ratio);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void sticky_keys(bool sticky);
    void set_mouse_position_visibility();
    void hide_cursor(bool hide);
};

#endif
