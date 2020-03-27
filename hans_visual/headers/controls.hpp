#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>
#include <cmath>
#include <iomanip>
#include <mutex>

#include "_options.hpp"

class controls;
extern controls *camera;
extern std::mutex cam_mut;

class controls
{
    GLFWwindow *window;

	// Time
	float lastTime = 0.0;
	float currentTime;
	float deltaTime;

	// Mouse
	double xpos, ypos, xpos0, ypos0;
    bool L_pressed, R_pressed, scroll_pressed;

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

    void computeMatricesFromInputs_FP(GLFWwindow*);
	void computeMatricesFromInputs_spherical(GLFWwindow*);

    enum cam_system{fp, sphere, plane, fps};

public:
    controls(int mode = CAM_MODE);
    //controls();
    //controls(GLFWwindow *win, int mode = CAM_MODE);
    //~controls() = default;

    glm::vec3 position;
    cam_system camera_mode;
	void adjustments(GLFWwindow *window);
	void computeMatricesFromInputs(GLFWwindow* window);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	friend void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	friend void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);

	// Points selection
	double sel_xpos, sel_ypos, sel_xpos0, sel_ypos0;
    void normalize_vec(glm::vec3 &vec);                             // Get the unitary vector
    double distance_sqr_vec(glm::vec3 &vec1, glm::vec3 &vec2);      // Square distance between 2 points
    bool R_just_released = false;
    bool is_R_pressed() { return R_pressed; }

    // Callback functions: Get mouse click - Get mouse out/in window - Get cursor position
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow *window, double xOffset, double yOffset);
};

#endif