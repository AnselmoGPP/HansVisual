#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "_options.hpp"
#include "controls.hpp"

class glfw_timer
{
    float lastTime = 0.0;
    float currentTime;
public:
    float deltaTime;
    void get_delta_time();  // Get time increment between two consecutives calls to this function
};

//enum cam_system{fp, sphere, plane, fps, zenith, sphere_z};

class camera
{
public:
    camera(glm::vec3 cam_pos, glm::vec3 cam_dir, glm::vec3 up_vec, float FV);
    virtual ~camera() = default;

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    virtual void computeMatricesFromInputs(float aspect_ratio) = 0;
    virtual void set_mouse_position_visibility() = 0;
    void hide_cursor(bool hide);
    void associate_window(GLFWwindow *window_in);

    glm::vec3 cam_position;
protected:
    glm::vec3 cam_direction;
    glm::vec3 up_vector;
    float FoV;

    GLFWwindow *window;
    keys_controller * kc;

    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    glfw_timer timer;
    float pi;

    void adjust_moves_to_range_2pi(float &lat, float &lon);
};

// Do have standard initial values for exclusive parameters. When changing camera mode, just change the common parameters (position, orientation)

class sphere : public camera
{
public:
    sphere();
    ~sphere() override = default;

    void computeMatricesFromInputs(float aspect_ratio) override;
    void set_mouse_position_visibility() override;
    void get_cursor_pos_increment();
    //void change_cam(float x, float y, float z, glm::vec3 direction);

    glm::vec3 sphere_center;
    float radius;
    float minimum_radius;
    float lat, lon;

    float mouse_speed;
    float scroll_speed;
    float right_speed;

    float xIncrem, yIncrem;
};

class firstPerson : public camera
{
public:
    firstPerson();
    ~firstPerson() override = default;

    void computeMatricesFromInputs(float aspect_ratio) override;
    void set_mouse_position_visibility() override;
    //void change_cam(float x, float y, float z, glm::vec3 direction);

    float lon;
    float lat;
    float walk_speed;                     // X units / second
    float mouse_speed;
};

#endif
